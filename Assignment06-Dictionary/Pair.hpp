#pragma once
#include <compare>

template <typename Key, typename Value>
struct Pair {
	Key first;
	Value second;

	auto operator<=>(const Pair &) const = default;
};
