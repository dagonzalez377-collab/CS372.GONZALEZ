#pragma once
#include <stdexcept>
#include <utility>

template <typename T>
class List {
protected:
    struct Node {
        T     data;
        Node* next{nullptr};
        explicit Node(const T& d) : data(d) {}
        explicit Node(T&& d)      : data(std::move(d)) {}
    };

    Node*  head_{nullptr};
    Node*  tail_{nullptr};
    size_t size_{0};

public:
    List() = default;

    List(const List& other) {
        for (Node* cur = other.head_; cur; cur = cur->next)
            push_back(cur->data);
    }

    List(List&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
    }

    List& operator=(const List& other) {
        if (this != &other) {
            List tmp(other);
            swap(tmp);
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }

    virtual ~List() { clear(); }

    virtual void push_front(const T& value) {
        Node* n = new Node(value);
        n->next = head_;
        head_   = n;
        if (!tail_) tail_ = head_;
        ++size_;
    }

    virtual void push_front(T&& value) {
        Node* n = new Node(std::move(value));
        n->next = head_;
        head_   = n;
        if (!tail_) tail_ = head_;
        ++size_;
    }

    virtual void push_back(const T& value) {
        Node* n = new Node(value);
        if (tail_) tail_->next = n;
        else       head_       = n;
        tail_ = n;
        ++size_;
    }

    virtual void push_back(T&& value) {
        Node* n = new Node(std::move(value));
        if (tail_) tail_->next = n;
        else       head_       = n;
        tail_ = n;
        ++size_;
    }

    virtual void pop_front() {
        if (!head_) throw std::underflow_error("pop_front on empty list");
        Node* old = head_;
        head_ = head_->next;
        if (!head_) tail_ = nullptr;
        delete old;
        --size_;
    }

    virtual void pop_back() {
        if (!head_) throw std::underflow_error("pop_back on empty list");
        if (head_ == tail_) {
            delete head_;
            head_ = tail_ = nullptr;
        } else {
            Node* cur = head_;
            while (cur->next != tail_) cur = cur->next;
            delete tail_;
            tail_       = cur;
            tail_->next = nullptr;
        }
        --size_;
    }

    virtual T& front() {
        if (!head_) throw std::underflow_error("front on empty list");
        return head_->data;
    }

    virtual const T& front() const {
        if (!head_) throw std::underflow_error("front on empty list");
        return head_->data;
    }

    virtual T& back() {
        if (!tail_) throw std::underflow_error("back on empty list");
        return tail_->data;
    }

    virtual const T& back() const {
        if (!tail_) throw std::underflow_error("back on empty list");
        return tail_->data;
    }

    virtual bool insert_after(const T& after_value, const T& new_value) {
        Node* cur = head_;
        while (cur) {
            if (cur->data == after_value) {
                Node* n   = new Node(new_value);
                n->next   = cur->next;
                cur->next = n;
                if (cur == tail_) tail_ = n;
                ++size_;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    virtual bool remove(const T& value) {
        if (!head_) return false;
        if (head_->data == value) { pop_front(); return true; }
        Node* prev = head_;
        while (prev->next) {
            if (prev->next->data == value) {
                Node* dead = prev->next;
                prev->next = dead->next;
                if (dead == tail_) tail_ = prev;
                delete dead;
                --size_;
                return true;
            }
            prev = prev->next;
        }
        return false;
    }

    virtual bool contains(const T& value) const {
        for (Node* cur = head_; cur; cur = cur->next)
            if (cur->data == value) return true;
        return false;
    }

    virtual void traverse(void (*visit)(const T&)) const {
        for (Node* cur = head_; cur; cur = cur->next)
            visit(cur->data);
    }

    size_t size()  const noexcept { return size_; }
    bool   empty() const noexcept { return size_ == 0; }

    virtual void clear() {
        while (head_) {
            Node* tmp = head_->next;
            delete head_;
            head_ = tmp;
        }
        tail_ = nullptr;
        size_ = 0;
    }

private:
    void swap(List& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(size_, other.size_);
    }
};
