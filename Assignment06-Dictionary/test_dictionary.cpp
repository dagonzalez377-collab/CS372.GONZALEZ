#include "Dictionary.hpp"
#include <iostream>
#include <string>
#include <cassert>

static void printDict(Dictionary<std::string, int> &d) {
	auto &ks = d.keys();
	auto &vs = d.values();
	auto ki = ks.begin();
	auto vi = vs.begin();
	std::cout << "{ ";
	while (ki != ks.end()) {
		std::cout << *ki << ":" << *vi << " ";
		++ki; ++vi;
	}
	std::cout << "}" << std::endl;
}

int main() {
	Pair<std::string, int> p1{"apple", 1};
	Pair<std::string, int> p2{"banana", 2};
	assert(p1 < p2);
	assert(!(p1 == p2));
	Pair<std::string, int> p1copy{"apple", 1};
	assert(p1 == p1copy);
	std::cout << "Pair comparisons OK" << std::endl;

	Dictionary<std::string, int> d;
	assert(d.isEmpty());
	assert(d.size() == 0);

	d.insert("banana", 2);
	d.insert("apple", 1);
	d.insert("cherry", 3);
	d.insert("date", 4);

	assert(d.size() == 4);
	assert(!d.isEmpty());
	assert(d.contains("apple"));
	assert(!d.contains("fig"));

	printDict(d);

	{
		auto &ks = d.keys();
		auto it = ks.begin();
		assert(*it == "apple"); ++it;
		assert(*it == "banana"); ++it;
		assert(*it == "cherry"); ++it;
		assert(*it == "date");
	}

	assert(d.at("banana") == 2);
	bool threw = false;
	try {
		d.at("nope");
	} catch (const std::out_of_range &) {
		threw = true;
	}
	assert(threw);
	std::cout << "at() OK" << std::endl;

	assert(d["apple"] == 1);
	d["apple"] = 100;
	assert(d.at("apple") == 100);
	assert(d.size() == 4);

	d["elderberry"] = 5;
	assert(d.size() == 5);
	assert(d.at("elderberry") == 5);
	std::cout << "operator[] OK" << std::endl;

	d.at("cherry") = 33;
	assert(d.at("cherry") == 33);
	std::cout << "at() as lvalue OK" << std::endl;

	d.erase("banana");
	assert(!d.contains("banana"));
	assert(d.size() == 4);
	printDict(d);

	Dictionary<int, char> d2;
	d2.insert(50, 'a');
	d2.insert(30, 'b');
	d2.insert(70, 'c');
	d2.insert(20, 'd');
	d2.insert(40, 'e');
	d2.insert(60, 'f');
	d2.insert(80, 'g');
	d2.erase(50);
	assert(!d2.contains(50));
	assert(d2.size() == 6);
	{
		auto &ks = d2.keys();
		int expected[] = {20, 30, 40, 60, 70, 80};
		int i = 0;
		for (auto it = ks.begin(); it != ks.end(); ++it, ++i) {
			assert(*it == expected[i]);
		}
	}
	std::cout << "erase() with two-child node OK" << std::endl;

	d.clear();
	assert(d.isEmpty());
	assert(d.size() == 0);
	std::cout << "clear() OK" << std::endl;

	Dictionary<std::string, int> d3;
	d3.insert("x", 1);
	d3.insert("y", 2);
	Dictionary<std::string, int> d4(d3);
	d4.insert("z", 3);
	assert(d3.size() == 2 && d4.size() == 3);

	Dictionary<std::string, int> d5;
	d5 = d3;
	assert(d5.size() == 2);

	Dictionary<std::string, int> d6(std::move(d4));
	assert(d6.size() == 3);

	Dictionary<std::string, int> d7;
	d7 = std::move(d6);
	assert(d7.size() == 3);

	std::cout << "Rule of Five OK" << std::endl;

	std::cout << "ALL TESTS PASSED" << std::endl;
	return 0;
}
