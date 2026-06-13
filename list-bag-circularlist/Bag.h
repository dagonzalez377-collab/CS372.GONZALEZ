#pragma once
#include "List.h"

template <typename T>
class Bag {
public:
    Bag() = default;
    Bag(const Bag&)            = default;
    Bag(Bag&&) noexcept        = default;
    Bag& operator=(const Bag&) = default;
    Bag& operator=(Bag&&)      = default;
    ~Bag()                     = default;

    void add(const T& item) { store_.push_front(item); }
    void add(T&& item)      { store_.push_front(std::move(item)); }

    bool remove(const T& item)      { return store_.remove(item); }
    bool contains(const T& item) const { return store_.contains(item); }

    size_t size()  const noexcept { return store_.size(); }
    bool   empty() const noexcept { return store_.empty(); }

    void clear()                          { store_.clear(); }
    void traverse(void (*visit)(const T&)) const { store_.traverse(visit); }

private:
    List<T> store_;
};
