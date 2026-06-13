#pragma once
#include "List.h"
#include <stdexcept>

template <typename T>
class CircularList : public List<T> {
    using Node = typename List<T>::Node;

public:
    CircularList() = default;
    CircularList(const CircularList&)            = default;
    CircularList(CircularList&&) noexcept        = default;
    CircularList& operator=(const CircularList&) = default;
    CircularList& operator=(CircularList&&)      = default;
    ~CircularList() override = default;

    void traverse(void (*visit)(const T&)) const override {
        if (!this->head_) return;
        traverse_from(this->head_, visit);
    }

    void traverse(const T& start_value, void (*visit)(const T&)) const {
        if (!this->head_)
            throw std::invalid_argument("traverse: list is empty");

        Node* start = this->head_;
        while (start && !(start->data == start_value))
            start = start->next;
        if (!start)
            throw std::invalid_argument("traverse: start_value not found");

        traverse_from(start, visit);
    }

private:
    void traverse_from(Node* start, void (*visit)(const T&)) const {
        Node* cur = start;
        do {
            visit(cur->data);
            cur = cur->next;
            if (!cur) cur = this->head_;
        } while (cur != start);
    }
};
