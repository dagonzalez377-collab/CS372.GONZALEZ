#include <iostream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include "Array.h"
#include "ReceiptBag.h"

// ── Utility ──────────────────────────────────
static int passed = 0, failed = 0;

void check(bool condition, const char* msg) {
    if (condition) { std::cout << "  PASS  " << msg << "\n"; ++passed; }
    else           { std::cout << "  FAIL  " << msg << "\n"; ++failed; }
}

// ─────────────────────────────────────────────
//  Array<T,N> Tests
// ─────────────────────────────────────────────
void testArray() {
    std::cout << "\n=== Array<T,N> Tests ===\n";

    // Basic construction and size
    Array<int, 5> a;
    check(a.size()     == 5,    "size() == N");
    check(a.capacity() == 5,    "capacity() == N");
    check(!a.empty(),           "empty() is false for N=5");

    // Zero-size edge case
    Array<int, 0> empty;
    check(empty.empty(), "empty() is true for N=0");

    // Element assignment and retrieval
    for (size_t i = 0; i < a.size(); ++i)
        a[i] = static_cast<int>(i * 10);
    check(a[0] == 0  && a[2] == 20 && a[4] == 40,
          "element assignment and retrieval");

    // front() / back()
    check(a.front() == 0  && a.back() == 40, "front() and back()");

    // fill()
    a.fill(7);
    bool allSeven = true;
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] != 7) { allSeven = false; break; }
    check(allSeven, "fill() sets all elements");

    // Copy constructor
    Array<int, 5> b(a);
    b[0] = 99;
    check(a[0] == 7 && b[0] == 99, "copy constructor is deep");

    // Assignment operator
    Array<int, 5> c;
    c = a;
    c[1] = 55;
    check(a[1] == 7 && c[1] == 55, "assignment operator is deep");

    // Out-of-range throws
    bool threw = false;
    try { (void)a[5]; } catch (const std::out_of_range&) { threw = true; }
    check(threw, "operator[] throws std::out_of_range");

    // Range-based for loop (iterator support)
    Array<int, 4> d;
    d[0]=1; d[1]=2; d[2]=3; d[3]=4;
    int sum = 0;
    for (int v : d) sum += v;
    check(sum == 10, "range-based for loop via begin()/end()");

    // String type
    Array<std::string, 3> s;
    s[0] = "alpha"; s[1] = "beta"; s[2] = "gamma";
    check(s[1] == "beta", "works with std::string elements");
}

// ─────────────────────────────────────────────
//  ReceiptBag<T> Tests
// ─────────────────────────────────────────────
void testReceiptBag() {
    std::cout << "\n=== ReceiptBag<T> Tests ===\n";

    ReceiptBag<int> bag;
    check(bag.empty(),        "new bag is empty");
    check(bag.size() == 0,    "new bag size == 0");

    // insert returns unique receipts
    int r1 = bag.insert(10);
    int r2 = bag.insert(20);
    int r3 = bag.insert(30);
    check(r1 != r2 && r2 != r3 && r1 != r3, "all receipts are unique");
    check(bag.size() == 3,                   "size after 3 inserts == 3");

    // contains
    check( bag.contains(r1), "contains() true for live receipt");
    check(!bag.contains(999),"contains() false for unknown receipt");

    // remove by receipt returns correct value
    int removed = bag.remove(r2);
    check(removed == 20,          "remove returns correct item (r2→20)");
    check(bag.size() == 2,        "size decrements after remove");
    check(!bag.contains(r2),      "receipt no longer valid after remove");

    // remaining items still accessible
    check(bag.contains(r1) && bag.contains(r3),
          "other receipts still valid after removal");

    // invalid receipt throws
    bool threw = false;
    try { bag.remove(999); }
    catch (const std::invalid_argument&) { threw = true; }
    check(threw, "remove with invalid receipt throws std::invalid_argument");

    // Remove everything
    bag.remove(r1);
    bag.remove(r3);
    check(bag.empty(), "bag is empty after removing all items");

    // Duplicate values — each gets its own receipt
    int ra = bag.insert(42);
    int rb = bag.insert(42);
    check(ra != rb,             "duplicate values get distinct receipts");
    check(bag.remove(ra) == 42, "remove first duplicate");
    check(bag.size() == 1,      "one duplicate remains");
    check(bag.remove(rb) == 42, "remove second duplicate");
    check(bag.empty(),          "bag empty after removing both duplicates");

    // Copy constructor test
    int rc = bag.insert(100);
    int rd = bag.insert(200);
    ReceiptBag<int> copy(bag);
    check(copy.size() == 2,   "copy constructor copies size");
    check(copy.contains(rc) && copy.contains(rd),
          "copy constructor copies receipts");
    copy.remove(rc);
    check( bag.contains(rc),  "removing from copy doesn't affect original");

    // String bag
    ReceiptBag<std::string> sbag;
    int sr1 = sbag.insert("hello");
    int sr2 = sbag.insert("world");
    check(sbag.remove(sr1) == "hello", "ReceiptBag works with std::string");
    check(sbag.remove(sr2) == "world", "second string item removed correctly");

    // Stress: trigger internal growth
    ReceiptBag<int> big;
    std::vector<int> recs;
    recs.reserve(1000);
    for (int i = 0; i < 1000; ++i)
        recs.push_back(big.insert(i));
    check(big.size() == 1000, "1000 inserts succeeded (growth triggered)");
    bool ok = true;
    for (int i = 999; i >= 0; --i)
        if (big.remove(recs[i]) != i) { ok = false; break; }
    check(ok,         "all 1000 items removed by receipt in reverse order");
    check(big.empty(),"bag empty after stress test");
}

int main() {
    testArray();
    testReceiptBag();

    std::cout << "\n─────────────────────────────────\n"
              << "  Results: " << passed << " passed, "
              << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
