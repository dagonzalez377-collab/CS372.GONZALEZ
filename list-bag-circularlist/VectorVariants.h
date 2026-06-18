#ifndef VECTOR_VARIANTS_H
#define VECTOR_VARIANTS_H

#include "Vector.h"
#include <algorithm>   // std::copy

// ─────────────────────────────────────────────────────────────
//  VectorSTLCopy<T>
//  Same ×2 growth policy as the base class, but replaces the
//  hand-written for-loop with std::copy.
// ─────────────────────────────────────────────────────────────
template <typename T>
class VectorSTLCopy : public Vector<T> {
protected:
    void copyElements(T* src, T* dst, size_t count) override {
        std::copy(src, src + count, dst);
    }
};

// ─────────────────────────────────────────────────────────────
//  VectorLargeStart<T>
//  Starts at capacity 128 and grows ×4 (quad instead of double).
//  Still uses the base for-loop copy – only the capacity policy
//  changes.
// ─────────────────────────────────────────────────────────────
template <typename T>
class VectorLargeStart : public Vector<T> {
public:
    VectorLargeStart()
        : Vector<T>() {
        // Replace the capacity-1 allocation with a capacity-128 one
        delete[] this->data_;
        this->data_     = new T[128];
        this->capacity_ = 128;
    }

protected:
    size_t newCapacity() const override {
        return this->capacity_ * 4;    // ×4 growth
    }
};

#endif // VECTOR_VARIANTS_H
