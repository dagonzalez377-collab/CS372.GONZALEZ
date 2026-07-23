#pragma once

#include "Graph.hpp"

#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename T>
class AdjMatrixGraph : public Graph<T>
{
private:
    std::vector<T> nodeOrder;
    std::unordered_map<T, int> indexOf;
    std::vector<std::vector<bool>> matrix;

public:
    AdjMatrixGraph() = default;

    AdjMatrixGraph(const std::vector<T> &nodes,
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
        if (indexOf.find(node) != indexOf.end())
        {
            return;
        }
        int newIndex = (int)nodeOrder.size();
        indexOf[node] = newIndex;
        nodeOrder.push_back(node);

        for (auto &row : matrix)
        {
            row.push_back(false);
        }
        matrix.push_back(std::vector<bool>(nodeOrder.size(), false));
    }

    void addEdge(const T &from, const T &to) override
    {
        addNode(from);
        addNode(to);
        matrix[indexOf[from]][indexOf[to]] = true;
    }

    void deleteEdge(const T &from, const T &to) override
    {
        auto fromIt = indexOf.find(from);
        auto toIt = indexOf.find(to);
        if (fromIt != indexOf.end() && toIt != indexOf.end())
        {
            matrix[fromIt->second][toIt->second] = false;
        }
    }

    bool adjacent(const T &from, const T &to) const override
    {
        auto fromIt = indexOf.find(from);
        auto toIt = indexOf.find(to);
        if (fromIt == indexOf.end() || toIt == indexOf.end())
        {
            return false;
        }
        return matrix[fromIt->second][toIt->second];
    }

    std::vector<T> neighbors(const T &node) const override
    {
        std::vector<T> result;
        auto it = indexOf.find(node);
        if (it == indexOf.end())
        {
            return result;
        }
        int row = it->second;
        int numNodes = (int)nodeOrder.size();
        for (int col = 0; col < numNodes; col++)
        {
            if (matrix[row][col])
            {
                result.push_back(nodeOrder[col]);
            }
        }
        return result;
    }

    bool hasNode(const T &node) const override
    {
        return indexOf.find(node) != indexOf.end();
    }

    void dfs(const T &start, const std::function<void(const T &)> &visit) const override
    {
        auto startIt = indexOf.find(start);
        if (startIt == indexOf.end())
        {
            return;
        }
        std::unordered_set<int> visited;
        dfsHelper(startIt->second, visit, visited);
    }

    void bfs(const T &start, const std::function<void(const T &)> &visit) const override
    {
        auto startIt = indexOf.find(start);
        if (startIt == indexOf.end())
        {
            return;
        }
        std::unordered_set<int> visited;
        std::queue<int> toVisit;
        toVisit.push(startIt->second);
        visited.insert(startIt->second);
        int numNodes = (int)nodeOrder.size();
        while (!toVisit.empty())
        {
            int current = toVisit.front();
            toVisit.pop();
            visit(nodeOrder[current]);
            for (int col = 0; col < numNodes; col++)
            {
                if (matrix[current][col] && visited.count(col) == 0)
                {
                    visited.insert(col);
                    toVisit.push(col);
                }
            }
        }
    }

private:
    void dfsHelper(int nodeIndex, const std::function<void(const T &)> &visit,
                    std::unordered_set<int> &visited) const
    {
        visited.insert(nodeIndex);
        visit(nodeOrder[nodeIndex]);
        int numNodes = (int)nodeOrder.size();
        for (int col = 0; col < numNodes; col++)
        {
            if (matrix[nodeIndex][col] && visited.count(col) == 0)
            {
                dfsHelper(col, visit, visited);
            }
        }
    }
};
