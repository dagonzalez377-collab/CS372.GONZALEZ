#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

void bubbleSort(std::vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void selectionSort(std::vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        std::swap(arr[i], arr[minIdx]);
    }
}

void insertionSort(std::vector<int> arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            std::swap(arr[++i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int> arr) {
    if (!arr.empty()) {
        quickSortHelper(arr, 0, arr.size() - 1);
    }
}

template <typename SortFn>
double timeSort(SortFn fn, const std::vector<int>& data) {
    auto start = std::chrono::high_resolution_clock::now();
    fn(data);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    std::vector<int> sizes = {10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 250000};

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 1000000);

    std::cout << std::left
              << std::setw(10) << "N"
              << std::setw(18) << "BubbleSort(ms)"
              << std::setw(20) << "SelectionSort(ms)"
              << std::setw(20) << "InsertionSort(ms)"
              << std::setw(16) << "QuickSort(ms)"
              << "\n";
    std::cout << std::string(84, '-') << "\n";

    for (int n : sizes) {
        std::vector<int> data(n);
        std::generate(data.begin(), data.end(), [&]{ return dist(rng); });

        auto fmt = [](double t) -> std::string {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(3) << t;
            return oss.str();
        };

        std::string bub, sel, ins;
        if (n <= 50000) {
            bub = fmt(timeSort(bubbleSort, data));
            sel = fmt(timeSort(selectionSort, data));
            ins = fmt(timeSort(insertionSort, data));
        } else {
            bub = sel = ins = "N/A";
        }

        std::string quick = fmt(timeSort(quickSort, data));

        std::cout << std::left
                  << std::setw(10) << n
                  << std::setw(18) << bub
                  << std::setw(20) << sel
                  << std::setw(20) << ins
                  << std::setw(16) << quick
                  << "\n";
    }

    return 0;
}
