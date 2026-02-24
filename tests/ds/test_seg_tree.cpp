#include "../framework/test_framework.hpp"
#include "cp/ds/seg_tree.hpp"

TEST_CASE(seg_tree_sum_build_and_query)
{
    cp::SegTree<cp::ll, std::plus<cp::ll>{}> st({1, 2, 3, 4, 5});
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(1, 3), 9LL);
    EXPECT_EQ(st.query(2, 2), 3LL);
}

TEST_CASE(seg_tree_sum_update)
{
    cp::SegTree<cp::ll, std::plus<cp::ll>{}> st({1, 2, 3, 4, 5});
    st.update(2, 10);
    EXPECT_EQ(st.query(0, 4), 22LL);
    EXPECT_EQ(st.query(1, 3), 16LL);
    EXPECT_EQ(st.query(2, 2), 10LL);
}

TEST_CASE(seg_tree_min_query)
{
    constexpr auto mn = [](cp::ll a, cp::ll b) { return std::min(a, b); };
    cp::SegTree<cp::ll, mn, cp::INF64> st({3, 1, 4, 1, 5});
    EXPECT_EQ(st.query(0, 4), 1LL);
    EXPECT_EQ(st.query(0, 0), 3LL);
    EXPECT_EQ(st.query(2, 4), 1LL);
}

TEST_CASE(seg_tree_min_update)
{
    constexpr auto mn = [](cp::ll a, cp::ll b) { return std::min(a, b); };
    cp::SegTree<cp::ll, mn, cp::INF64> st({3, 1, 4, 1, 5});
    st.update(1, 10);
    st.update(3, 10);
    EXPECT_EQ(st.query(0, 4), 3LL);
    EXPECT_EQ(st.query(1, 3), 4LL);
}

TEST_CASE(seg_tree_default_constructed)
{
    cp::SegTree<cp::ll, std::plus<cp::ll>{}> st(5);
    EXPECT_EQ(st.query(0, 4), 0LL);
    st.update(2, 7);
    EXPECT_EQ(st.query(0, 4), 7LL);
    EXPECT_EQ(st.query(0, 1), 0LL);
}

TEST_CASE(seg_tree_assertions)
{
    cp::SegTree<cp::ll, std::plus<cp::ll>{}> st(5);
    EXPECT_ABORT(st.update(-1, 1));
    EXPECT_ABORT(st.update(5, 1));
    EXPECT_ABORT(st.query(-1, 2));
    EXPECT_ABORT(st.query(2, 5));
    EXPECT_ABORT(st.query(3, 1));
}
