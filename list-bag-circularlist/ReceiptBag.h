#ifndef RECEIPT_BAG_H
#define RECEIPT_BAG_H

#include <stdexcept>
#include <cstddef>

// ─────────────────────────────────────────────────────────────
//  ReceiptBag<T>
//  A bag whose insert() returns a unique integer receipt.
//  remove(receipt) locates the matching item via its receipt,
//  removes it, and returns the item by value.
//
//  Implementation uses two parallel dynamic arrays:
//    items_[]    – the stored elements
//    receipts_[] – the receipt assigned to each element
//
//  Deleted slots are handled by swap-with-last so the arrays
//  stay dense (no holes).
// ─────────────────────────────────────────────────────────────
template <typename T>
class ReceiptBag {
public:
    // ── Constructors / Destructor ──────────────
    explicit ReceiptBag(size_t initialCapacity = 8)
        : items_(new T[initialCapacity]),
          receipts_(new int[initialCapacity]),
          size_(0),
          capacity_(initialCapacity),
          nextReceipt_(0) {}

    ReceiptBag(const ReceiptBag& other)
        : items_(new T[other.capacity_]),
          receipts_(new int[other.capacity_]),
          size_(other.size_),
          capacity_(other.capacity_),
          nextReceipt_(other.nextReceipt_) {
        for (size_t i = 0; i < size_; ++i) {
            items_[i]    = other.items_[i];
            receipts_[i] = other.receipts_[i];
        }
    }

    ReceiptBag& operator=(const ReceiptBag& other) {
        if (this != &other) {
            delete[] items_;
            delete[] receipts_;
            capacity_    = other.capacity_;
            size_        = other.size_;
            nextReceipt_ = other.nextReceipt_;
            items_       = new T[capacity_];
            receipts_    = new int[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                items_[i]    = other.items_[i];
                receipts_[i] = other.receipts_[i];
            }
        }
        return *this;
    }

    ~ReceiptBag() {
        delete[] items_;
        delete[] receipts_;
    }

    // ── insert ────────────────────────────────
    // Adds the item and returns a unique receipt integer.
    int insert(const T& item) {
        if (size_ == capacity_)
            grow();
        int receipt        = nextReceipt_++;
        items_[size_]      = item;
        receipts_[size_]   = receipt;
        ++size_;
        return receipt;
    }

    // ── remove ────────────────────────────────
    // Finds the slot with the matching receipt, removes it
    // (swap-with-last to keep the array dense), and returns
    // the item by value.
    T remove(int receipt) {
        int slot = findSlot(receipt);
        if (slot < 0)
            throw std::invalid_argument("ReceiptBag: invalid receipt");
        T removed = items_[slot];
        // Swap with last element and shrink
        items_[slot]    = items_[size_ - 1];
        receipts_[slot] = receipts_[size_ - 1];
        --size_;
        return removed;
    }

    // ── contains ──────────────────────────────
    // Returns true if a live item with this receipt exists.
    bool contains(int receipt) const {
        return findSlot(receipt) >= 0;
    }

    // ── Accessors ─────────────────────────────
    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }

private:
    // ── Helpers ───────────────────────────────
    int findSlot(int receipt) const {
        for (size_t i = 0; i < size_; ++i)
            if (receipts_[i] == receipt)
                return static_cast<int>(i);
        return -1;
    }

    void grow() {
        size_t newCap    = capacity_ * 2;
        T*     newItems  = new T[newCap];
        int*   newRecpts = new int[newCap];
        for (size_t i = 0; i < size_; ++i) {
            newItems[i]  = items_[i];
            newRecpts[i] = receipts_[i];
        }
        delete[] items_;
        delete[] receipts_;
        items_    = newItems;
        receipts_ = newRecpts;
        capacity_ = newCap;
    }

    T*     items_;
    int*   receipts_;
    size_t size_;
    size_t capacity_;
    int    nextReceipt_;   // monotonically increasing receipt counter
};

#endif // RECEIPT_BAG_H
