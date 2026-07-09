#pragma once
#include "Pair.hpp"
#include "Tree.hpp"
#include "List.hpp"
#include <stdexcept>
#include <utility>
#include <cstddef>

template <typename Key, typename Value>
class Dictionary {
public:
	using PairType = Pair<Key, Value>;

private:
	struct KeyLess {
		bool operator()(const PairType &a, const PairType &b) const {
			return a.first < b.first;
		}
	};

	Tree<PairType> _tree;
	std::size_t _size = 0;

	List<Key> _keysCache;
	List<Value> _valuesCache;

	static PairType probeFor(const Key &key) {
		return PairType{ key, Value{} };
	}

	bool containsKey(const Key &key) const {
		return _tree.member(probeFor(key), KeyLess{});
	}

	Value lookupValue(const Key &key) const {
		Tree<PairType> found;
		if (!_tree.find(probeFor(key), found, KeyLess{})) {
			throw std::out_of_range("Dictionary::at: key not found");
		}
		return found.root().second;
	}

	static Tree<PairType> removeKey(const Tree<PairType> &t, const Key &key) {
		if (t.isEmpty()) return t;

		PairType y = t.root();
		KeyLess less;
		PairType probe = probeFor(key);

		if (less(probe, y)) {
			return Tree<PairType>(removeKey(t.left(), key), y, t.right());
		} else if (less(y, probe)) {
			return Tree<PairType>(t.left(), y, removeKey(t.right(), key));
		}

		if (t.left().isEmpty())  return t.right();
		if (t.right().isEmpty()) return t.left();

		Tree<PairType> successorSubtree = t.right();
		while (!successorSubtree.left().isEmpty()) {
			successorSubtree = successorSubtree.left();
		}
		PairType successor = successorSubtree.root();
		Tree<PairType> newRight = removeKey(t.right(), successor.first);
		return Tree<PairType>(t.left(), successor, newRight);
	}

	class ValueProxy {
		Dictionary *_dict;
		Key _key;

	public:
		ValueProxy(Dictionary *dict, Key key) : _dict(dict), _key(std::move(key)) {}

		operator Value() const {
			return _dict->lookupValue(_key);
		}

		ValueProxy & operator=(const Value &v) {
			_dict->insert(_key, v);
			return *this;
		}

		ValueProxy & operator=(const ValueProxy &rhs) {
			return *this = static_cast<Value>(rhs);
		}
	};

public:
	Dictionary() = default;

	Dictionary(const Dictionary &rhs)
		: _tree(rhs._tree), _size(rhs._size) {}

	Dictionary & operator=(const Dictionary &rhs) {
		if (this != &rhs) {
			_tree = rhs._tree;
			_size = rhs._size;
			_keysCache = List<Key>();
			_valuesCache = List<Value>();
		}
		return *this;
	}

	Dictionary(Dictionary &&rhs) noexcept
		: _tree(std::move(rhs._tree)), _size(rhs._size) {
		rhs._size = 0;
	}

	Dictionary & operator=(Dictionary &&rhs) noexcept {
		if (this != &rhs) {
			_tree = std::move(rhs._tree);
			_size = rhs._size;
			rhs._size = 0;
			_keysCache = List<Key>();
			_valuesCache = List<Value>();
		}
		return *this;
	}

	~Dictionary() = default;

	ValueProxy operator[](const Key &key) {
		if (!containsKey(key)) {
			insert(key, Value{});
		}
		return ValueProxy(this, key);
	}

	Value at(const Key &key) const {
		return lookupValue(key);
	}

	ValueProxy at(const Key &key) {
		if (!containsKey(key)) {
			throw std::out_of_range("Dictionary::at: key not found");
		}
		return ValueProxy(this, key);
	}

	void insert(const Key &key, const Value &value) {
		bool existed = containsKey(key);
		if (existed) {
			_tree = removeKey(_tree, key);
		}
		_tree = _tree.insert(PairType{ key, value }, KeyLess{});
		if (!existed) ++_size;
	}

	void insert(const PairType &p) {
		insert(p.first, p.second);
	}

	void erase(const Key &key) {
		if (containsKey(key)) {
			_tree = removeKey(_tree, key);
			--_size;
		}
	}

	void clear() {
		_tree = Tree<PairType>();
		_size = 0;
	}

	bool contains(const Key &key) const {
		return containsKey(key);
	}

	List<Key> & keys() {
		_keysCache = List<Key>();
		_tree.inorder([this](PairType p) { _keysCache.push_back(p.first); });
		return _keysCache;
	}

	List<Value> & values() {
		_valuesCache = List<Value>();
		_tree.inorder([this](PairType p) { _valuesCache.push_back(p.second); });
		return _valuesCache;
	}

	bool isEmpty() const { return _tree.isEmpty(); }
	std::size_t size() const { return _size; }
};
