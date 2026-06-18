#ifndef ARRAY_H
#define ARRAY_H

#include <stdexcept>
#include <cstddef>   // size_t

// ─────────────────────────────────────────────────────────────
//  Array<T, N>
//  Fixed-size wrapper around a raw C++ array.
//  Parameterised by element type T and compile-time size N.
//  Public interface mirrors Vector<T> where sensible.
// ─────────────────────────────────────────────────────────────
template <typename T, size_t N>
class Array {
public:
    // ── Constructors ──────────────────────────
    Array() : size_(N) {}

    Array(const Array& other) : size_(N) {
        for (size_t i = 0; i < N; ++i)
            data_[i] = other.data_[i];
    }

    Array& operator=(const Array& other) {
        if (this != &other)
            for (size_t i = 0; i < N; ++i)
                data_[i] = other.data_[i];
        return *this;
    }

    ~Array() = default;

    // ── Element access ────────────────────────
    T& operator[](size_t index) {
        if (index >= N)
            throw std::out_of_range("Array index out of range");
        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= N)
            throw std::out_of_range("Array index out of range");
        return data_[index];
    }

    T& at(size_t index)             { return (*this)[index]; }
    const T& at(size_t index) const { return (*this)[index]; }

    T&       front()       { return data_[0]; }
    const T& front() const { return data_[0]; }
    T&       back()        { return data_[N - 1]; }
    const T& back()  const { return data_[N - 1]; }

    // ── Capacity ──────────────────────────────
    size_t size()     const { return N; }
    size_t capacity() const { return N; }          // fixed; matches Vector API
    bool   empty()    const { return N == 0; }

    // ── Raw pointer access ────────────────────
    T*       data()       { return data_; }
    const T* data() const { return data_; }

    // ── Simple iterators ─────────────────────
    T*       begin()       { return data_; }
    const T* begin() const { return data_; }
    T*       end()         { return data_ + N; }
    const T* end()   const { return data_ + N; }

    // ── Fill helper ───────────────────────────
    void fill(const T& value) {
        for (size_t i = 0; i < N; ++i)
            data_[i] = value;
    }

private:
    T      data_[N];
    size_t size_;    // always N; kept for API parity with Vector
};

#endif // ARRAY_H
