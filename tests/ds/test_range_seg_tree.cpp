#include "../framework/test_framework.hpp"
#include "cp/ds/range_seg_tree.hpp"

TEST_CASE(sum_range_seg_tree_build_and_query)
{
    cp::SumRangeSegTree<cp::ll> st({1, 2, 3, 4, 5});
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(1, 3), 9LL);
    EXPECT_EQ(st.query(2, 2), 3LL);
}

TEST_CASE(sum_range_seg_tree_range_update)
{
    cp::SumRangeSegTree<cp::ll> st({1, 2, 3, 4, 5});
    st.update(1, 3, 10);
    // [1, 12, 13, 14, 5]
    EXPECT_EQ(st.query(0, 4), 45LL);
    EXPECT_EQ(st.query(1, 3), 39LL);
    EXPECT_EQ(st.query(0, 0), 1LL); // unchanged
    EXPECT_EQ(st.query(4, 4), 5LL); // unchanged
}

TEST_CASE(sum_range_seg_tree_overlapping_updates)
{
    cp::SumRangeSegTree<cp::ll> st(5);
    st.update(0, 4, 1); // [1, 1, 1, 1, 1]
    st.update(1, 3, 2); // [1, 3, 3, 3, 1]
    st.update(2, 2, 3); // [1, 3, 6, 3, 1]
    EXPECT_EQ(st.query(0, 4), 14LL);
    EXPECT_EQ(st.query(1, 3), 12LL);
    EXPECT_EQ(st.query(2, 2), 6LL);
}

TEST_CASE(sum_range_seg_tree_default_constructed)
{
    cp::SumRangeSegTree<cp::ll> st(5);
    EXPECT_EQ(st.query(0, 4), 0LL);
    st.update(2, 2, 7);
    EXPECT_EQ(st.query(0, 4), 7LL);
    EXPECT_EQ(st.query(0, 1), 0LL);
}

TEST_CASE(sum_range_seg_tree_assertions)
{
    cp::SumRangeSegTree<cp::ll> st(5);
    EXPECT_ABORT(st.update(-1, 2, 1));
    EXPECT_ABORT(st.update(0, 5, 1));
    EXPECT_ABORT(st.update(3, 1, 1));
    EXPECT_ABORT(st.query(-1, 2));
    EXPECT_ABORT(st.query(0, 5));
    EXPECT_ABORT(st.query(3, 1));
}
