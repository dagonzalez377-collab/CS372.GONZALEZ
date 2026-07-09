//
// File:   List.hpp
// Author: Your Glorious Instructor
// Purpose:
// Implement a doubly linked list class with iterators.
//
// Refactored to use smart pointers for node ownership: each node OWNS the
// node after it (next is a unique_ptr), and just OBSERVES the node before
// it (prev is a raw pointer). That split is necessary -- if both directions
// were owning pointers, every node would keep its neighbor alive forever
// and the list would never free itself.
//
#pragma once
#include <functional>
#include <iostream>
#include <memory>

template <typename T>
class List {
private:
	class Node {
	public:
		T data;
		std::unique_ptr<Node> next;
		Node *prev = nullptr;
		bool isHiddenNode = false;

		Node() = default;
		Node(T d, Node *p) : data(std::move(d)), prev(p) {}
	};

	std::unique_ptr<Node> head;  // owns the whole chain, head -> ... -> tail
	Node *tail = nullptr;        // just an observer into that chain

public:
	class const_iterator {
		protected:
			Node *current;
			T & retrieve() const { return current->data; }
			const_iterator(Node *p) : current(p) { }
			friend class List<T>;

		public:
			const_iterator(): current(nullptr) { }

			virtual const T & operator*() const {
				return retrieve();
			}

			const_iterator & operator++() {
				current = current->next.get();
				return *this;
			}

			const_iterator operator++(int) {
				const_iterator old = *this;
				++(*this);
				return old;
			}

			bool operator==(const const_iterator & rhs) const {
				return current == rhs.current;
			}
			bool operator!=(const const_iterator & rhs) const {
				return !(*this == rhs);
			}
	};

public:
	class iterator: public const_iterator {
		protected:
			iterator(Node *p) : const_iterator(p) { }
			friend class List<T>;

		public:
			iterator() { }

			T & operator*() {
				return const_iterator::retrieve();
			}

			const T & operator*() const {
				return const_iterator::operator*();
			}

			iterator & operator++() {
				this->current = const_iterator::current->next.get();
				return *this;
			}

			iterator operator++(int) {
				iterator old = *this;
				++(*this);
				return old;
			}
	};

private:
	// Splice a fresh node in right after head -- used the first time
	// something is pushed onto an empty list.
	void setupList() {
		auto newNode = std::make_unique<Node>();
		newNode->prev = head.get();
		newNode->next = std::move(head->next);   // newNode now owns the tail sentinel
		newNode->next->prev = newNode.get();
		head->next = std::move(newNode);         // head now owns newNode
	}

	// Reset back to the empty state (head -> tail) without destroying the
	// sentinels themselves.
	//
	// IMPORTANT: this must unlink nodes one at a time in a loop, not just
	// drop the head of the chain and let unique_ptr cascade-destroy the
	// rest. Each Node's destructor destroys its own `next` member, so a
	// cascade means one C++ function call per node -- for a long list that
	// blows the call stack (verified with ASan on a 200k-node list).
	// Detaching iteratively keeps destruction O(n) in a single stack frame.
	void deleteListContents() {
		if (!tail) return; // moved-from state -- nothing to clean up
		std::unique_ptr<Node> current = std::move(head->next);
		while (current && current.get() != tail) {
			current = std::move(current->next); // old 'current' node dies here, but its
			                                      // own next was already released above,
			                                      // so this single destruction can't cascade
		}
		head->next = std::move(current); // hands the tail sentinel back to head
		tail->prev = head.get();
	}

public:
	List() {
		head = std::make_unique<Node>();
		head->isHiddenNode = true;
		head->prev = nullptr;

		auto tailNode = std::make_unique<Node>();
		tailNode->isHiddenNode = true;
		tailNode->prev = head.get();

		tail = tailNode.get();
		head->next = std::move(tailNode);
	}

	List(T newData) : List() {
		setupList();
		head->next->data = std::move(newData);
	}

	// Rule of five.

	List(const List &rhs) : List() {
		for (Node *curr = rhs.head->next.get(); curr != rhs.tail; curr = curr->next.get()) {
			push_back(curr->data);
		}
	}

	List & operator=(const List &rhs) {
		if (this != &rhs) {
			List temp(rhs);          // build a real copy
			*this = std::move(temp); // and steal it -- reuses move assignment below
		}
		return *this;
	}

	List(List &&rhs) noexcept
		: head(std::move(rhs.head)), tail(rhs.tail) {
		rhs.tail = nullptr;
	}

	List & operator=(List &&rhs) noexcept {
		if (this != &rhs) {
			deleteListContents(); // empty *this* out iteratively first --
			                       // otherwise the plain assignment below
			                       // would drop our old (possibly huge)
			                       // chain all at once and risk the same
			                       // recursive-destructor stack overflow
			head->next = std::move(rhs.head->next);
			tail = rhs.tail;
			if (head->next) head->next->prev = head.get();
			rhs.tail = nullptr;
		}
		return *this;
	}

	virtual ~List() {
		deleteListContents(); // iterative -- safe for arbitrarily long lists
	}

	bool empty() const {
		return (head->next.get() == tail);
	}

	iterator begin() const { return iterator{ head->next.get() }; }
	iterator end() const { return iterator{ tail }; }

	const_iterator cbegin() const { return { head->next.get() }; }
	const_iterator cend() const { return { tail }; }

	iterator insert(iterator itr, const T & x) {
		Node *p = itr.current;
		Node *prevNode = p->prev;

		auto newNode = std::make_unique<Node>(x, prevNode);
		Node *newNodeRaw = newNode.get();

		newNode->next = std::move(prevNode->next); // newNode now owns p
		newNode->next->prev = newNodeRaw;
		prevNode->next = std::move(newNode);       // prevNode now owns newNode

		return iterator{ newNodeRaw };
	}

	iterator erase(iterator itr) {
		Node *p = itr.current;
		Node *nextNode = p->next.get();
		Node *prevNode = p->prev;

		iterator iterToReturn{ nextNode };

		nextNode->prev = prevNode;
		prevNode->next = std::move(p->next); // drops p, prevNode now owns nextNode

		return iterToReturn;
	}

	iterator erase(iterator from, iterator to) {
		iterator itr = from;
		while (itr != to) {
			itr = erase(itr);
		}
		return to;
	}

	void push_front(T data) {
		if (empty()) {
			setupList();
			head->next->data = std::move(data);
		} else {
			auto newNode = std::make_unique<Node>(std::move(data), head.get());
			Node *newNodeRaw = newNode.get();

			newNode->next = std::move(head->next); // newNode now owns old first node
			newNode->next->prev = newNodeRaw;
			head->next = std::move(newNode);        // head now owns newNode
		}
	}

	void push_back(T data) {
		if (empty()) {
			setupList();
			tail->prev->data = std::move(data);
		} else {
			Node *oldTailPrev = tail->prev;
			auto newNode = std::make_unique<Node>(std::move(data), oldTailPrev);
			Node *newNodeRaw = newNode.get();

			newNode->next = std::move(oldTailPrev->next); // newNode now owns tail sentinel
			newNode->next->prev = newNodeRaw;
			oldTailPrev->next = std::move(newNode);        // oldTailPrev now owns newNode
		}
	}

	T front() {
		return head->next->data;
	}

	T back() {
		return tail->prev->data;
	}

	void pop_back() {
		if (!empty()) {
			Node *lastNode = tail->prev;
			Node *newLastNode = lastNode->prev;
			tail->prev = newLastNode;
			newLastNode->next = std::move(lastNode->next); // drops lastNode
		} else {
			std::cerr << "pop_back(): Attempt to pop from empty list. " << std::endl;
		}
	}

	void pop_front() {
		if (!empty()) {
			Node *firstNode = head->next.get();
			Node *newFirstNode = firstNode->next.get();
			newFirstNode->prev = head.get();
			head->next = std::move(firstNode->next); // drops firstNode
		} else {
			std::cerr << "pop_front(): Attempt to pop from empty list. " << std::endl;
		}
	}

	void traverse(std::function<void(T &data)> doIt) {
		Node *current = head->next.get();
		while (current != tail) {
			doIt(current->data);
			current = current->next.get();
		}
	}
};
