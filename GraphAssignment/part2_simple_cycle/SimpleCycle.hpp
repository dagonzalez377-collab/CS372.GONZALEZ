#pragma once

#include "Graph.hpp"

#include <algorithm>
#include <vector>

inline bool isValidPath(const std::vector<int> &path, Graph<int> &g)
{
    for (int i = 0; i + 1 < (int)path.size(); i++)
    {
        std::vector<int> neighVec = g.neighbors(path[i]);
        if (std::find(neighVec.begin(), neighVec.end(), path[i + 1]) == neighVec.end())
        {
            return false;
        }
    }
    return true;
}

inline bool isSimpleCycle(Graph<int> &g, const std::vector<int> &path)
{
    if (path.size() < 3)
    {
        return false;
    }
    if (path.front() != path.back())
    {
        return false;
    }
    if (!isValidPath(path, g))
    {
        return false;
    }

    // Make sure no node (other than the repeated start/end) shows up twice.
    for (int i = 0; i + 1 < (int)path.size(); i++)
    {
        for (int j = i + 1; j + 1 < (int)path.size(); j++)
        {
            if (path[i] == path[j])
            {
                return false;
            }
        }
    }
    return true;
}
