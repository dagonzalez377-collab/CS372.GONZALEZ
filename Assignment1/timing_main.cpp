#include <iostream>
#include <chrono>
#include <iomanip>
#include "Vector.h"
#include "VectorVariants.h"

// ─────────────────────────────────────────────────────────────
//  timeVector
//  Takes any Vector<int>* (base or derived) through the pointer
//  and times adding 'targetSize' integers to it.
//  Uses polymorphism: push_back / grow / copyElements are all
//  virtual, so no changes to this function are needed for
//  derived classes.
// ─────────────────────────────────────────────────────────────
double timeVector(Vector<int>* v, size_t targetSize) {
    using Clock = std::chrono::high_resolution_clock;
    auto start  = Clock::now();
    for (size_t i = 0; i < targetSize; ++i)
        v->push_back(static_cast<int>(i));
    auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    // Column header
    std::cout << std::left
              << std::setw(12) << "N"
              << std::setw(20) << "Base (for-loop)"
              << std::setw(20) << "STL copy"
              << std::setw(20) << "Large-start x4"
              << "\n";
    std::cout << std::string(72, '-') << "\n";

    // Powers of two: 2^1 … 2^22  (~4 million)
    for (int exp = 1; exp <= 22; ++exp) {
        size_t n = static_cast<size_t>(1) << exp;   // 2^exp

        Vector<int>          base;
        VectorSTLCopy<int>   stlV;
        VectorLargeStart<int> largeV;

        double tBase  = timeVector(&base,  n);
        double tSTL   = timeVector(&stlV,  n);
        double tLarge = timeVector(&largeV, n);

        std::cout << std::left
                  << std::setw(12) << n
                  << std::setw(20) << std::fixed << std::setprecision(4) << tBase
                  << std::setw(20) << tSTL
                  << std::setw(20) << tLarge
                  << "\n";
    }
    return 0;
}
