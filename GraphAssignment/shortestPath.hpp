#pragma once

#include <algorithm>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Graph.hpp"

template <typename T>
std::vector<int> shortestPath(Graph<T> &aGraph, int start, int destination)
{
    if (!aGraph.hasNode(start) || !aGraph.hasNode(destination))
    {
        throw std::out_of_range("shortestPath: start or destination is not a vertex in the graph");
    }

    if (start == destination)
    {
        return std::vector<int>{start};
    }

    std::unordered_set<T> visited;
    std::unordered_map<T, T> predecessor;
    std::queue<T> toVisit;

    visited.insert(start);
    toVisit.push(start);

    bool found = false;
    while (!toVisit.empty() && !found)
    {
        T current = toVisit.front();
        toVisit.pop();

        for (const T &nb : aGraph.neighbors(current))
        {
            if (visited.count(nb) == 0)
            {
                visited.insert(nb);
                predecessor[nb] = current;
                if (nb == destination)
                {
                    found = true;
                    break;
                }
                toVisit.push(nb);
            }
        }
    }

    if (!found)
    {
        return std::vector<int>();
    }

    std::vector<int> path;
    T node = destination;
    while (!(node == start))
    {
        path.push_back(node);
        node = predecessor[node];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}
