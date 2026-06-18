#ifndef LAZYLINKEDLIST_H
#define LAZYLINKEDLIST_H

#include <iostream>
using namespace std;

template <typename T>
class LazyLinkedList {
private:
    struct Node {
        T data;
        bool deleted;
        Node* next;

        Node(T val) : data(val), deleted(false), next(nullptr) {}
    };

    Node* head;
    int activeCount;
    int deletedCount;

    void purge() {
        while (head != nullptr && head->deleted) {
            Node* temp = head;
            head = head->next;
            delete temp;
            deletedCount--;
        }

        Node* curr = head;
        while (curr != nullptr && curr->next != nullptr) {
            if (curr->next->deleted) {
                Node* temp = curr->next;
                curr->next = temp->next;
                delete temp;
                deletedCount--;
            } else {
                curr = curr->next;
            }
        }
    }

public:
    LazyLinkedList() : head(nullptr), activeCount(0), deletedCount(0) {}

    ~LazyLinkedList() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    void insert(T val) {
        Node* newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        activeCount++;
    }

    void remove(T val) {
        Node* curr = head;
        while (curr != nullptr) {
            if (!curr->deleted && curr->data == val) {
                curr->deleted = true;
                activeCount--;
                deletedCount++;
                if (deletedCount >= activeCount && deletedCount > 0) {
                    purge();
                }
                return;
            }
            curr = curr->next;
        }
        cout << "Value " << val << " not found in list." << endl;
    }

    bool search(T val) const {
        Node* curr = head;
        while (curr != nullptr) {
            if (!curr->deleted && curr->data == val)
                return true;
            curr = curr->next;
        }
        return false;
    }

    void print() const {
        Node* curr = head;
        bool first = true;
        while (curr != nullptr) {
            if (!curr->deleted) {
                if (!first) cout << " -> ";
                cout << curr->data;
                first = false;
            }
            curr = curr->next;
        }
        cout << endl;
    }

    int size() const {
        return activeCount;
    }

    int hiddenSize() const {
        return deletedCount;
    }

    bool isEmpty() const {
        return activeCount == 0;
    }
};

#endif
