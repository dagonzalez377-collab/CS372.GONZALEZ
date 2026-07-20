#pragma once

#include "AdjListGraph.hpp"

#include <algorithm>
#include <queue>
#include <string>
#include <vector>

inline AdjListGraph<std::string> buildSocialGraph()
{
    AdjListGraph<std::string> g;

    std::vector<std::string> people{
        "David", "Maria", "Ben", "Priya", "Jordan", "Sam", "Aisha",
        "Tom", "Lucia", "Noah", "Grace", "Marcus", "Elena", "Ryan",
        "Nina", "Chris", "Fatima", "Owen", "Zara", "Kevin Bacon"};
    for (const auto &p : people)
    {
        g.addNode(p);
    }

    std::vector<std::pair<std::string, std::string>> friendships{
        {"David", "Maria"},
        {"David", "Ben"},
        {"David", "Priya"},
        {"Maria", "Kevin Bacon"},
        {"Maria", "Jordan"},
        {"Ben", "Sam"},
        {"Ben", "Aisha"},
        {"Priya", "Tom"},
        {"Priya", "Lucia"},
        {"Jordan", "Noah"},
        {"Jordan", "Grace"},
        {"Sam", "Marcus"},
        {"Aisha", "Elena"},
        {"Tom", "Ryan"},
        {"Lucia", "Nina"},
        {"Noah", "Chris"},
        {"Grace", "Fatima"},
        {"Marcus", "Owen"},
        {"Elena", "Zara"},
        {"Elena", "Kevin Bacon"},
        {"Ryan", "Chris"},
        {"Nina", "Owen"},
        {"Chris", "Zara"},
        {"Fatima", "Owen"},
    };
    for (const auto &edge : friendships)
    {
        g.addEdge(edge.first, edge.second);
        g.addEdge(edge.second, edge.first);
    }

    return g;
}

inline bool isConnectedToKevinBacon(Graph<std::string> &g,
                                     const std::string &me,
                                     const std::string &kevinBacon,
                                     std::vector<std::string> &path)
{
    path.clear();

    if (me == kevinBacon)
    {
        path.push_back(me);
        return true;
    }

    // Plain BFS using parallel vectors instead of hash maps/sets.
    std::vector<std::string> visited;
    std::vector<std::string> parent;
    std::queue<std::string> toVisit;

    toVisit.push(me);
    visited.push_back(me);
    parent.push_back("");

    bool found = false;
    while (!toVisit.empty() && !found)
    {
        std::string current = toVisit.front();
        toVisit.pop();

        std::vector<std::string> neighborList = g.neighbors(current);
        for (int i = 0; i < (int)neighborList.size(); i++)
        {
            std::string nb = neighborList[i];
            if (std::find(visited.begin(), visited.end(), nb) == visited.end())
            {
                visited.push_back(nb);
                parent.push_back(current);
                if (nb == kevinBacon)
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
        return false;
    }

    std::string node = kevinBacon;
    path.push_back(node);
    while (node != me)
    {
        int idx = -1;
        for (int i = 0; i < (int)visited.size(); i++)
        {
            if (visited[i] == node)
            {
                idx = i;
                break;
            }
        }
        node = parent[idx];
        path.push_back(node);
    }
    std::reverse(path.begin(), path.end());
    return true;
}

inline int kevinBaconNumber(const std::vector<std::string> &path)
{
    if (path.empty())
    {
        return -1;
    }
    return (int)path.size() - 1;
}
