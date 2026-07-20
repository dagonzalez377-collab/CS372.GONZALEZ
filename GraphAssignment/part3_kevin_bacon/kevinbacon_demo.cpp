#include <iostream>

#include "SocialGraph.hpp"

int main()
{
    AdjListGraph<std::string> g = buildSocialGraph();

    std::vector<std::string> path;
    bool connected = isConnectedToKevinBacon(g, "David", "Kevin Bacon", path);

    if (connected)
    {
        std::cout << "Connected to Kevin Bacon! Path:\n  ";
        for (std::size_t i = 0; i < path.size(); ++i)
        {
            std::cout << path[i];
            if (i + 1 < path.size())
            {
                std::cout << " -> ";
            }
        }
        std::cout << "\nKevin Bacon number: " << kevinBaconNumber(path) << "\n";
    }
    else
    {
        std::cout << "Not connected to Kevin Bacon.\n";
    }

    return 0;
}
