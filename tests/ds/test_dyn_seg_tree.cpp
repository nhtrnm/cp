#include "../framework/test_framework.hpp"
#include "cp/ds/dyn_seg_tree.hpp"

TEST_CASE(dyn_seg_tree_range_add_and_sum)
{
    cp::DynSegTree<cp::ll> st(0, 9);
    st.update(2, 5, 3);
    EXPECT_EQ(st.query(0, 9), 12LL);
    EXPECT_EQ(st.query(2, 5), 12LL);
    EXPECT_EQ(st.query(0, 1), 0LL);
    EXPECT_EQ(st.query(6, 9), 0LL);
}

TEST_CASE(dyn_seg_tree_large_range)
{
    cp::DynSegTree<cp::ll> st(0LL, 1000000000LL);
    st.update(0LL, 1000000000LL, 1LL);
    EXPECT_EQ(st.query(0LL, 1000000000LL), 1000000001LL);
    st.update(500000000LL, 500000000LL, 5LL);
    EXPECT_EQ(st.query(500000000LL, 500000000LL), 6LL);
}

TEST_CASE(dyn_seg_tree_overlapping_updates)
{
    cp::DynSegTree<cp::ll> st(0, 9);
    st.update(0, 9, 1); // [0..9] += 1
    st.update(2, 7, 2); // [2..7] += 2
    st.update(4, 5, 3); // [4..5] += 3
    // [0,1]=1, [2,3]=3, [4,5]=6, [6,7]=3, [8,9]=1
    EXPECT_EQ(st.query(0, 9), 28LL);
    EXPECT_EQ(st.query(4, 5), 12LL);
    EXPECT_EQ(st.query(0, 1), 2LL);
}

TEST_CASE(dyn_seg_tree_negative_range)
{
    cp::DynSegTree<cp::ll> st(-5, 5);
    st.update(-5, -1, 2);
    st.update(1, 5, 3);
    EXPECT_EQ(st.query(-5, 5), 25LL); // 5 * 2 + 5 * 3
    EXPECT_EQ(st.query(-5, -1), 10LL);
    EXPECT_EQ(st.query(0, 0), 0LL);
    EXPECT_EQ(st.query(1, 5), 15LL);
}

TEST_CASE(dyn_seg_tree_assertions)
{
    cp::DynSegTree<cp::ll> st(0, 9);
    EXPECT_ABORT(st.update(-1, 5, 1));
    EXPECT_ABORT(st.update(0, 10, 1));
    EXPECT_ABORT(st.update(5, 3, 1));
    EXPECT_ABORT(st.query(-1, 5));
    EXPECT_ABORT(st.query(0, 10));
    EXPECT_ABORT(st.query(5, 3));
    EXPECT_ABORT((cp::DynSegTree<cp::ll>(5, 3)));
}

TEST_CASE(dyn_seg_tree_const_query)
{
    cp::DynSegTree<cp::ll> st(0, 9);
    st.update(0, 9, 2);
    const cp::DynSegTree<cp::ll> &cst = st;
    EXPECT_EQ(cst.query(0, 9), 20LL);
    EXPECT_EQ(cst.query(3, 6), 8LL);
}
