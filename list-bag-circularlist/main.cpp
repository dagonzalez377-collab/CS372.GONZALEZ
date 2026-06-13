#include <iostream>
#include <string>
#include <cassert>
#include "List.h"
#include "Bag.h"
#include "CircularList.h"

static std::string collected;

void collect(const int& v)         { collected += std::to_string(v) + " "; }
void collectS(const std::string& s) { collected += s + " "; }

#define PASS(msg) std::cout << "  [PASS] " << msg << "\n"
#define SECTION(s) std::cout << "\n=== " << s << " ===\n"

void test_list_basic() {
    SECTION("List<int> - basic ADT");

    List<int> lst;
    assert(lst.empty());
    assert(lst.size() == 0);
    PASS("default construct");

    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);
    assert(lst.size() == 3);
    assert(lst.front() == 10);
    assert(lst.back()  == 30);
    PASS("push_back, front, back");

    lst.push_front(5);
    assert(lst.front() == 5);
    PASS("push_front");

    lst.pop_front();
    assert(lst.front() == 10);
    PASS("pop_front");

    lst.pop_back();
    assert(lst.back() == 20);
    PASS("pop_back");

    assert( lst.contains(10));
    assert(!lst.contains(99));
    PASS("contains");

    lst.insert_after(10, 15);
    collected.clear();
    lst.traverse(collect);
    assert(collected == "10 15 20 ");
    PASS("insert_after + traverse");

    assert( lst.remove(15));
    assert(!lst.remove(999));
    PASS("remove");

    lst.clear();
    assert(lst.empty());
    PASS("clear");
}

void test_list_copy_move() {
    SECTION("List<int> - copy & move");

    List<int> a;
    a.push_back(1); a.push_back(2); a.push_back(3);

    List<int> b(a);
    assert(b.size() == 3);
    b.push_back(4);
    assert(a.size() == 3);
    PASS("copy constructor");

    List<int> c;
    c = a;
    c.pop_front();
    assert(a.front() == 1);
    PASS("copy assignment");

    List<int> d(std::move(a));
    assert(d.size() == 3);
    assert(a.empty());
    PASS("move constructor");

    List<int> e;
    e = std::move(d);
    assert(e.size() == 3);
    assert(d.empty());
    PASS("move assignment");
}

void test_list_edge() {
    SECTION("List<int> - edge cases");

    List<int> lst;

    bool threw = false;
    try { lst.pop_front(); } catch (const std::underflow_error&) { threw = true; }
    assert(threw);
    PASS("pop_front on empty throws");

    threw = false;
    try { lst.pop_back(); } catch (const std::underflow_error&) { threw = true; }
    assert(threw);
    PASS("pop_back on empty throws");

    threw = false;
    try { lst.front(); } catch (const std::underflow_error&) { threw = true; }
    assert(threw);
    PASS("front on empty throws");

    lst.push_back(42);
    lst.pop_back();
    assert(lst.empty());
    PASS("pop_back single element");
}

void test_bag() {
    SECTION("Bag<string>");

    Bag<std::string> bag;
    bag.add("apple");
    bag.add("banana");
    bag.add("apple");
    assert(bag.size() == 3);
    assert( bag.contains("apple"));
    assert(!bag.contains("cherry"));
    PASS("add / contains / size");

    bag.remove("apple");
    assert(bag.size() == 2);
    assert(bag.contains("apple"));
    PASS("remove one occurrence");

    bag.remove("apple");
    assert(!bag.contains("apple"));
    PASS("remove last occurrence");

    assert(!bag.remove("nope"));
    PASS("remove missing returns false");

    Bag<std::string> bag2(bag);
    bag2.add("durian");
    assert(bag.size() != bag2.size());
    PASS("copy constructor");

    Bag<std::string> bag3(std::move(bag));
    assert(bag.empty());
    PASS("move constructor");

    Bag<std::string> bag4;
    bag4.add("x"); bag4.add("y"); bag4.add("z");
    collected.clear();
    bag4.traverse(collectS);
    assert(collected.find("x") != std::string::npos);
    assert(collected.find("y") != std::string::npos);
    assert(collected.find("z") != std::string::npos);
    PASS("traverse");
}

void test_circular_list() {
    SECTION("CircularList<int>");

    CircularList<int> cl;
    cl.push_back(1); cl.push_back(2); cl.push_back(3);
    cl.push_back(4); cl.push_back(5);

    collected.clear();
    cl.traverse(collect);
    assert(collected == "1 2 3 4 5 ");
    PASS("traverse() from head");

    collected.clear();
    cl.traverse(1, collect);
    assert(collected == "1 2 3 4 5 ");
    PASS("traverse(1) wraps back to start");

    collected.clear();
    cl.traverse(3, collect);
    assert(collected == "3 4 5 1 2 ");
    PASS("traverse(3) mid-list start");

    collected.clear();
    cl.traverse(5, collect);
    assert(collected == "5 1 2 3 4 ");
    PASS("traverse(5) tail start");

    CircularList<int> single;
    single.push_back(42);
    collected.clear();
    single.traverse(42, collect);
    assert(collected == "42 ");
    PASS("single element");

    bool threw = false;
    try { cl.traverse(99, collect); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    PASS("missing start throws");

    CircularList<int> empty;
    threw = false;
    try { empty.traverse(1, collect); } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
    PASS("empty list throws");

    List<int>* ptr = &cl;
    collected.clear();
    ptr->traverse(collect);
    assert(collected == "1 2 3 4 5 ");
    PASS("polymorphic dispatch");
}

int main() {
    std::cout << "=== Test Suite ===\n";
    test_list_basic();
    test_list_copy_move();
    test_list_edge();
    test_bag();
    test_circular_list();
    std::cout << "\nAll tests passed!\n";
    return 0;
}
