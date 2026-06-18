#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
#include <algorithm>  // std::copy

// ─────────────────────────────────────────────
//  Base Vector<T>
//  • starts with capacity 1, doubles on overflow
//  • uses a raw for-loop to copy (overridable)
// ─────────────────────────────────────────────
template <typename T>
class Vector {
public:
    // ── Constructors / Destructor ──────────────
    Vector()
        : data_(new T[1]), size_(0), capacity_(1) {}

    Vector(const Vector& other)
        : data_(new T[other.capacity_]),
          size_(other.size_),
          capacity_(other.capacity_) {
        copyElements(other.data_, data_, size_);
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            capacity_ = other.capacity_;
            size_     = other.size_;
            data_     = new T[capacity_];
            copyElements(other.data_, data_, size_);
        }
        return *this;
    }

    virtual ~Vector() { delete[] data_; }

    // ── Mutators ──────────────────────────────
    virtual void push_back(const T& value) {
        if (size_ == capacity_)
            grow();
        data_[size_++] = value;
    }

    void pop_back() {
        if (size_ == 0)
            throw std::underflow_error("Vector is empty");
        --size_;
    }

    // ── Accessors ─────────────────────────────
    T& operator[](size_t index) {
        if (index >= size_)
            throw std::out_of_range("Vector index out of range");
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_)
            throw std::out_of_range("Vector index out of range");
        return data_[index];
    }

    size_t size()     const { return size_; }
    size_t capacity() const { return capacity_; }
    bool   empty()    const { return size_ == 0; }

protected:
    // ── Internal helpers (virtual → subclasses can override) ──
    virtual void grow() {
        size_t newCap  = newCapacity();
        T*     newData = new T[newCap];
        copyElements(data_, newData, size_);   // virtual copy step
        delete[] data_;
        data_     = newData;
        capacity_ = newCap;
    }

    // Returns the next capacity value – override to change growth strategy
    virtual size_t newCapacity() const {
        return capacity_ * 2;          // ×2 doubling (base policy)
    }

    // Copies 'count' elements src→dst – override to change copy strategy
    virtual void copyElements(T* src, T* dst, size_t count) {
        for (size_t i = 0; i < count; ++i)
            dst[i] = src[i];
    }

    T*     data_;
    size_t size_;
    size_t capacity_;
};

#endif // VECTOR_H
