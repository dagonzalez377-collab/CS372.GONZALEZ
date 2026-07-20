#include <gtest/gtest.h>

#include "SocialGraph.hpp"

class KevinBaconTest : public ::testing::Test
{
protected:
    AdjListGraph<std::string> g = buildSocialGraph();
};

TEST_F(KevinBaconTest, DavidIsConnected)
{
    std::vector<std::string> path;
    bool connected = isConnectedToKevinBacon(g, "David", "Kevin Bacon", path);
    EXPECT_TRUE(connected);
    std::vector<std::string> expected{"David", "Maria", "Kevin Bacon"};
    EXPECT_EQ(path, expected);
    EXPECT_EQ(kevinBaconNumber(path), 2);
}

TEST_F(KevinBaconTest, EveryoneInTheGraphIsConnected)
{
    std::vector<std::string> people{
        "David", "Maria", "Ben", "Priya", "Jordan", "Sam", "Aisha",
        "Tom", "Lucia", "Noah", "Grace", "Marcus", "Elena", "Ryan",
        "Nina", "Chris", "Fatima", "Owen", "Zara"};
    for (const auto &person : people)
    {
        std::vector<std::string> path;
        EXPECT_TRUE(isConnectedToKevinBacon(g, person, "Kevin Bacon", path))
            << person << " should be connected to Kevin Bacon";
        EXPECT_FALSE(path.empty());
        EXPECT_EQ(path.front(), person);
        EXPECT_EQ(path.back(), "Kevin Bacon");
    }
}

TEST_F(KevinBaconTest, KevinBaconNumberOfKevinBaconIsZero)
{
    std::vector<std::string> path;
    bool connected = isConnectedToKevinBacon(g, "Kevin Bacon", "Kevin Bacon", path);
    EXPECT_TRUE(connected);
    EXPECT_EQ(kevinBaconNumber(path), 0);
}

TEST_F(KevinBaconTest, UnknownPersonIsNotConnected)
{
    std::vector<std::string> path;
    bool connected = isConnectedToKevinBacon(g, "Nobody McNoFriends", "Kevin Bacon", path);
    EXPECT_FALSE(connected);
    EXPECT_TRUE(path.empty());
    EXPECT_EQ(kevinBaconNumber(path), -1);
}

TEST_F(KevinBaconTest, DisconnectedComponentReturnsFalse)
{
    AdjListGraph<std::string> isolated;
    isolated.addNode("Alice");
    isolated.addNode("Zoe");
    isolated.addEdge("Alice", "Zoe");
    isolated.addEdge("Zoe", "Alice");
    isolated.addNode("Kevin Bacon");

    std::vector<std::string> path;
    bool connected = isConnectedToKevinBacon(isolated, "Alice", "Kevin Bacon", path);
    EXPECT_FALSE(connected);
    EXPECT_TRUE(path.empty());
}
