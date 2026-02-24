#include "../framework/test_framework.hpp"
#include "cp/ds/dsu.hpp"

TEST_CASE(dsu_initial_state)
{
    cp::DSU d(5);
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(d.find(i), i);
        EXPECT_TRUE(d.same(i, i));
    }
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 5; j++) {
            EXPECT_FALSE(d.same(i, j));
        }
    }
}

TEST_CASE(dsu_merge_and_same)
{
    cp::DSU d(5);
    d.merge(0, 1);
    EXPECT_TRUE(d.same(0, 1));
    EXPECT_FALSE(d.same(0, 2));

    d.merge(2, 3);
    EXPECT_TRUE(d.same(2, 3));
    EXPECT_FALSE(d.same(1, 3));

    d.merge(1, 3);
    EXPECT_TRUE(d.same(0, 3));
    EXPECT_FALSE(d.same(0, 4));
}

TEST_CASE(dsu_sizes)
{
    cp::DSU d(5);
    EXPECT_EQ(d.sizes[d.find(0)], 1);

    d.merge(0, 1);
    EXPECT_EQ(d.sizes[d.find(0)], 2);

    d.merge(0, 2);
    EXPECT_EQ(d.sizes[d.find(0)], 3);

    d.merge(3, 4);
    EXPECT_EQ(d.sizes[d.find(3)], 2);

    d.merge(0, 3);
    EXPECT_EQ(d.sizes[d.find(0)], 5);
}

TEST_CASE(dsu_merge_already_same)
{
    cp::DSU d(4);
    d.merge(0, 1);
    d.merge(0, 1); // merging again should be a no-op
    EXPECT_TRUE(d.same(0, 1));
    EXPECT_EQ(d.sizes[d.find(0)], 2); // size should still be 2, not 4
    EXPECT_FALSE(d.same(0, 2));
    EXPECT_FALSE(d.same(0, 3));
}
