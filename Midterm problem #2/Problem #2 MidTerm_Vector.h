#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>
#include <algorithm>

template <typename T>
class Vector {
public:

    class iterator {
    public:
        iterator(T* data, size_t index) : data_(data), index_(index) {}

        T& operator*() {
            return data_[index_];
        }

        iterator& operator++() {
            ++index_;
            return *this;
        }

        bool operator==(const iterator& other) const {
            return index_ == other.index_;
        }

        bool operator!=(const iterator& other) const {
            return index_ != other.index_;
        }

    private:
        T* data_;
        size_t index_;
    };

    iterator begin() { return iterator(data_, 0); }
    iterator end()   { return iterator(data_, size_); }

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

    virtual void grow() {
        size_t newCap  = newCapacity();
        T*     newData = new T[newCap];
        copyElements(data_, newData, size_);
        delete[] data_;
        data_     = newData;
        capacity_ = newCap;
    }

    virtual size_t newCapacity() const {
        return capacity_ * 2;
    }

    virtual void copyElements(T* src, T* dst, size_t count) {
        for (size_t i = 0; i < count; ++i)
            dst[i] = src[i];
    }

    T*     data_;
    size_t size_;
    size_t capacity_;
};

#endif
