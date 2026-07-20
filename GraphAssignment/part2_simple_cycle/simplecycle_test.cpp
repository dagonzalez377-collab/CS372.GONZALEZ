#include <gtest/gtest.h>

#include "AdjListGraph.hpp"
#include "SimpleCycle.hpp"

class TriangleGraphTest : public ::testing::Test
{
protected:
    AdjListGraph<int> g;
    void SetUp() override
    {
        g.addNode(0);
        g.addNode(1);
        g.addNode(2);

        g.addEdge(0, 1);
        g.addEdge(1, 0);
        g.addEdge(0, 2);
        g.addEdge(2, 0);
        g.addEdge(1, 2);
        g.addEdge(2, 1);
    }
};

TEST_F(TriangleGraphTest, ValidSimpleCycleIsTrue)
{
    std::vector<int> path{0, 1, 2, 0};
    EXPECT_TRUE(isSimpleCycle(g, path));
}

TEST_F(TriangleGraphTest, ValidSimpleCycleOtherDirectionIsTrue)
{
    std::vector<int> path{0, 2, 1, 0};
    EXPECT_TRUE(isSimpleCycle(g, path));
}

TEST_F(TriangleGraphTest, RepeatedInteriorNodeIsFalse)
{
    std::vector<int> path{0, 1, 0, 2, 0};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

TEST_F(TriangleGraphTest, NotClosedIsFalse)
{
    std::vector<int> path{0, 1, 2};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

TEST_F(TriangleGraphTest, TooShortIsFalse)
{
    std::vector<int> path{0, 1};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

TEST_F(TriangleGraphTest, SingleRepeatedNodeIsFalse)
{
    std::vector<int> path{0, 0};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

TEST_F(TriangleGraphTest, EmptyPathIsFalse)
{
    std::vector<int> path{};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

class SquareGraphTest : public ::testing::Test
{
protected:
    AdjListGraph<int> g;
    void SetUp() override
    {
        g.addNode(0);
        g.addNode(1);
        g.addNode(2);
        g.addNode(3);

        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        g.addEdge(3, 0);
    }
};

TEST_F(SquareGraphTest, FullLoopIsSimpleCycle)
{
    std::vector<int> path{0, 1, 2, 3, 0};
    EXPECT_TRUE(isSimpleCycle(g, path));
}

TEST_F(SquareGraphTest, WrongDirectionIsFalse)
{
    std::vector<int> path{0, 3, 2, 1, 0};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

TEST_F(SquareGraphTest, SkippingANodeIsFalse)
{
    std::vector<int> path{0, 2, 3, 0};
    EXPECT_FALSE(isSimpleCycle(g, path));
}

TEST_F(SquareGraphTest, PartialPathIsNotClosed)
{
    std::vector<int> path{0, 1, 2, 3};
    EXPECT_FALSE(isSimpleCycle(g, path));
}
