#pragma once

#include <functional>
#include <vector>

template <typename T>
class Graph
{
public:
    virtual ~Graph() = default;

    virtual void addNode(const T &node) = 0;
    virtual void addEdge(const T &from, const T &to) = 0;
    virtual void deleteEdge(const T &from, const T &to) = 0;
    virtual bool adjacent(const T &from, const T &to) const = 0;
    virtual std::vector<T> neighbors(const T &node) const = 0;
    virtual void dfs(const T &start, const std::function<void(const T &)> &visit) const = 0;
    virtual void bfs(const T &start, const std::function<void(const T &)> &visit) const = 0;

    virtual bool hasNode(const T &node) const = 0;
};
