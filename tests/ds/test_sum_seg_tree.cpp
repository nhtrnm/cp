#include "../framework/test_framework.hpp"
#include "cp/ds/seg_tree.hpp"

TEST_CASE(sum_seg_tree_build_and_query)
{
    cp::LongSumSegTree st({1, 2, 3, 4, 5});
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(1, 3), 9LL);
    EXPECT_EQ(st.query(2, 2), 3LL);
}

TEST_CASE(sum_seg_tree_update)
{
    cp::LongSumSegTree st({1, 2, 3, 4, 5});
    st.update(2, 10);
    EXPECT_EQ(st.query(0, 4), 22LL);
    EXPECT_EQ(st.query(1, 3), 16LL);
    EXPECT_EQ(st.query(2, 2), 10LL);
}

TEST_CASE(sum_seg_tree_default_constructed)
{
    cp::LongSumSegTree st(5);
    EXPECT_EQ(st.query(0, 4), 0LL);
    st.update(2, 7);
    EXPECT_EQ(st.query(0, 4), 7LL);
    EXPECT_EQ(st.query(0, 1), 0LL);
}

TEST_CASE(sum_seg_tree_assertions)
{
    cp::LongSumSegTree st(5);
    EXPECT_ABORT(st.update(-1, 1));
    EXPECT_ABORT(st.update(5, 1));
    EXPECT_ABORT(st.query(-1, 2));
    EXPECT_ABORT(st.query(2, 5));
    EXPECT_ABORT(st.query(3, 1));
}
