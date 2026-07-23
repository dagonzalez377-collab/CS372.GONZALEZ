#pragma once

#include <list>
#include <queue>
#include "Tree.hpp"

template <typename T>
std::list<T>* fringe(const Tree<T> &t) {
    std::list<T> *result = new std::list<T>();
    std::queue<Tree<T>> q;
    if (!t.isEmpty()) {
        q.push(t);
    }
    while (!q.empty()) {
        Tree<T> current = q.front();
        q.pop();
        if (current.isLeaf()) {
            result->push_back(current.root());
        } else {
            if (!current.left().isEmpty())  q.push(current.left());
            if (!current.right().isEmpty()) q.push(current.right());
        }
    }
    return result;
}

template <typename T>
bool hasSameFringe(const Tree<T> &t1, const Tree<T> &t2) {
    std::list<T> *f1 = fringe(t1);
    std::list<T> *f2 = fringe(t2);
    bool same = (*f1 == *f2);
    delete f1;
    delete f2;
    return same;
}
