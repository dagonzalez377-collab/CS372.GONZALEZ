#pragma once

#include "Graph.hpp"

#include <algorithm>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename T>
class AdjListGraph : public Graph<T>
{
private:
    std::vector<T> nodeOrder;
    std::unordered_map<T, std::list<T>> adjList;

public:
    AdjListGraph() = default;

    AdjListGraph(const std::vector<T> &nodes,
                 const std::vector<std::pair<T, T>> &edges)
    {
        for (const auto &n : nodes)
        {
            addNode(n);
        }
        for (const auto &e : edges)
        {
            addEdge(e.first, e.second);
        }
    }

    void addNode(const T &node) override
    {
        if (adjList.find(node) == adjList.end())
        {
            adjList.emplace(node, std::list<T>());
            nodeOrder.push_back(node);
        }
    }

    void addEdge(const T &from, const T &to) override
    {
        addNode(from);
        addNode(to);
        auto &neighborList = adjList[from];
        if (std::find(neighborList.begin(), neighborList.end(), to) == neighborList.end())
        {
            neighborList.push_back(to);
        }
    }

    void deleteEdge(const T &from, const T &to) override
    {
        auto it = adjList.find(from);
        if (it != adjList.end())
        {
            it->second.remove(to);
        }
    }

    bool adjacent(const T &from, const T &to) const override
    {
        auto it = adjList.find(from);
        if (it == adjList.end())
        {
            return false;
        }
        return std::find(it->second.begin(), it->second.end(), to) != it->second.end();
    }

    std::vector<T> neighbors(const T &node) const override
    {
        std::vector<T> result;
        auto it = adjList.find(node);
        if (it != adjList.end())
        {
            result.assign(it->second.begin(), it->second.end());
        }
        return result;
    }

    void dfs(const T &start, const std::function<void(const T &)> &visit) const override
    {
        if (adjList.find(start) == adjList.end())
        {
            return;
        }
        std::unordered_set<T> visited;
        dfsHelper(start, visit, visited);
    }

    void bfs(const T &start, const std::function<void(const T &)> &visit) const override
    {
        if (adjList.find(start) == adjList.end())
        {
            return;
        }
        std::unordered_set<T> visited;
        std::queue<T> toVisit;
        toVisit.push(start);
        visited.insert(start);
        while (!toVisit.empty())
        {
            T current = toVisit.front();
            toVisit.pop();
            visit(current);
            auto it = adjList.find(current);
            if (it != adjList.end())
            {
                for (const auto &nb : it->second)
                {
                    if (visited.count(nb) == 0)
                    {
                        visited.insert(nb);
                        toVisit.push(nb);
                    }
                }
            }
        }
    }

private:
    void dfsHelper(const T &node, const std::function<void(const T &)> &visit,
                    std::unordered_set<T> &visited) const
    {
        visited.insert(node);
        visit(node);
        auto it = adjList.find(node);
        if (it != adjList.end())
        {
            for (const auto &neighbor : it->second)
            {
                if (visited.count(neighbor) == 0)
                {
                    dfsHelper(neighbor, visit, visited);
                }
            }
        }
    }
};
