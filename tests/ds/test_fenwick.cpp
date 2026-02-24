#include "../framework/test_framework.hpp"
#include "cp/ds/fenwick.hpp"

TEST_CASE(fenwick_prefix_sum)
{
    // [1, 2, 3, 4, 5]
    cp::Fenwick<int> fw({1, 2, 3, 4, 5});
    EXPECT_EQ(fw.query(0), 1);
    EXPECT_EQ(fw.query(2), 6);  // 1+2+3
    EXPECT_EQ(fw.query(4), 15); // 1+2+3+4+5
}

TEST_CASE(fenwick_range_query)
{
    cp::Fenwick<int> fw({1, 2, 3, 4, 5});
    EXPECT_EQ(fw.query(1, 3), 9);  // 2+3+4
    EXPECT_EQ(fw.query(0, 4), 15); // entire array
    EXPECT_EQ(fw.query(2, 2), 3);  // single element
}

TEST_CASE(fenwick_point_update)
{
    cp::Fenwick<int> fw(5);
    fw.add(0, 1);
    fw.add(1, 2);
    fw.add(2, 3);
    fw.add(3, 4);
    fw.add(4, 5);
    EXPECT_EQ(fw.query(4), 15);
    fw.add(2, 10);
    EXPECT_EQ(fw.query(2), 16);    // 1+2+13
    EXPECT_EQ(fw.query(1, 3), 19); // 2+13+4
}

TEST_CASE(fenwick_long_long)
{
    cp::Fenwick<cp::ll> fw(3);
    fw.add(0, cp::INF64);
    fw.add(1, cp::INF64);
    EXPECT_EQ(fw.query(0, 1), 2 * cp::INF64);
}

TEST_CASE(fenwick_query_single_at_zero)
{
    cp::Fenwick<int> fw({1, 2, 3, 4, 5});
    EXPECT_EQ(fw.query(0, 0), 1);
}

TEST_CASE(offset_fenwick_prefix_sum)
{
    // array [1, 2, 3, 4, 5] over indices [3, 7]
    cp::OffsetFenwick<int> fw(3, {1, 2, 3, 4, 5});
    EXPECT_EQ(fw.query(3), 1);  // prefix [3,3] = 1
    EXPECT_EQ(fw.query(5), 6);  // prefix [3,5] = 1+2+3
    EXPECT_EQ(fw.query(7), 15); // prefix [3,7] = 1+2+3+4+5
}

TEST_CASE(offset_fenwick_range_query)
{
    cp::OffsetFenwick<int> fw(3, {1, 2, 3, 4, 5});
    EXPECT_EQ(fw.query(4, 6), 9);  // 2+3+4
    EXPECT_EQ(fw.query(3, 7), 15); // entire range
    EXPECT_EQ(fw.query(5, 5), 3);  // single element
}

TEST_CASE(offset_fenwick_point_update)
{
    cp::OffsetFenwick<int> fw(3, 7);
    fw.add(3, 1);
    fw.add(4, 2);
    fw.add(5, 3);
    fw.add(6, 4);
    fw.add(7, 5);
    EXPECT_EQ(fw.query(3, 7), 15);
    fw.add(5, 10);
    EXPECT_EQ(fw.query(3, 5), 16); // 1+2+13
}
