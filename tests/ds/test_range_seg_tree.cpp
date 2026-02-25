#include "../framework/test_framework.hpp"
#include "cp/ds/range_seg_tree.hpp"

TEST_CASE(range_seg_tree_sum_add_build_and_query)
{
    cp::RangeSegTree<cp::LongSumAddPolicy> st({1, 2, 3, 4, 5});
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(1, 3), 9LL);
    EXPECT_EQ(st.query(0, 0), 1LL);
}

TEST_CASE(range_seg_tree_sum_add_range_update)
{
    cp::RangeSegTree<cp::LongSumAddPolicy> st({1, 2, 3, 4, 5});
    st.update(1, 3, 10);
    EXPECT_EQ(st.query(0, 4), 45LL);
    EXPECT_EQ(st.query(1, 3), 39LL);
    EXPECT_EQ(st.query(0, 0), 1LL);
    EXPECT_EQ(st.query(4, 4), 5LL);
}

TEST_CASE(range_seg_tree_sum_add_overlapping_updates)
{
    cp::RangeSegTree<cp::LongSumAddPolicy> st(5);
    st.update(0, 4, 1);
    st.update(1, 3, 2);
    st.update(2, 2, 3);
    // [0]=1, [1]=3, [2]=6, [3]=3, [4]=1
    EXPECT_EQ(st.query(0, 4), 14LL);
    EXPECT_EQ(st.query(2, 2), 6LL);
}

TEST_CASE(range_seg_tree_min_add_build_and_query)
{
    cp::RangeSegTree<cp::LongMinAddPolicy> st({3, 1, 4, 1, 5});
    EXPECT_EQ(st.query(0, 4), 1LL);
    EXPECT_EQ(st.query(0, 2), 1LL);
    EXPECT_EQ(st.query(2, 4), 1LL);
    EXPECT_EQ(st.query(2, 2), 4LL);
}

TEST_CASE(range_seg_tree_min_add_range_update)
{
    cp::RangeSegTree<cp::LongMinAddPolicy> st({3, 1, 4, 1, 5});
    st.update(0, 4, 2); // {5, 3, 6, 3, 7}
    EXPECT_EQ(st.query(0, 4), 3LL);
    st.update(1, 3, -5); // {5, -2, 1, -2, 7}
    EXPECT_EQ(st.query(0, 4), -2LL);
    EXPECT_EQ(st.query(0, 0), 5LL);
    EXPECT_EQ(st.query(2, 2), 1LL);
}

TEST_CASE(range_seg_tree_min_add_overlapping_updates)
{
    cp::RangeSegTree<cp::LongMinAddPolicy> st(5);
    st.update(0, 4, 3);
    st.update(1, 3, -4); // [0]=3, [1]=-1, [2]=-1, [3]=-1, [4]=3
    EXPECT_EQ(st.query(0, 4), -1LL);
    EXPECT_EQ(st.query(0, 0), 3LL);
    EXPECT_EQ(st.query(4, 4), 3LL);
}

TEST_CASE(range_seg_tree_sum_set_build_and_query)
{
    cp::RangeSegTree<cp::LongSumSetPolicy> st({1, 2, 3, 4, 5});
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(1, 3), 9LL);
}

TEST_CASE(range_seg_tree_sum_set_range_update)
{
    cp::RangeSegTree<cp::LongSumSetPolicy> st({1, 2, 3, 4, 5});
    st.update(1, 3, 10); // {1, 10, 10, 10, 5}
    EXPECT_EQ(st.query(0, 4), 36LL);
    EXPECT_EQ(st.query(1, 3), 30LL);
    EXPECT_EQ(st.query(0, 0), 1LL);
    EXPECT_EQ(st.query(4, 4), 5LL);
}

TEST_CASE(range_seg_tree_sum_set_overlapping_updates)
{
    cp::RangeSegTree<cp::LongSumSetPolicy> st(5);
    st.update(0, 4, 2); // {2, 2, 2, 2, 2}
    st.update(2, 2, 7); // {2, 2, 7, 2, 2}
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(2, 2), 7LL);
    st.update(1, 3, 1); // {2, 1, 1, 1, 2} - later set overwrites
    EXPECT_EQ(st.query(0, 4), 7LL);
    EXPECT_EQ(st.query(2, 2), 1LL);
}

TEST_CASE(range_seg_tree_min_set_build_and_query)
{
    cp::RangeSegTree<cp::LongMinSetPolicy> st({3, 1, 4, 1, 5});
    EXPECT_EQ(st.query(0, 4), 1LL);
    EXPECT_EQ(st.query(0, 2), 1LL);
    EXPECT_EQ(st.query(3, 4), 1LL);
}

TEST_CASE(range_seg_tree_min_set_range_update)
{
    cp::RangeSegTree<cp::LongMinSetPolicy> st({3, 1, 4, 1, 5});
    st.update(0, 4, 3); // {3, 3, 3, 3, 3}
    EXPECT_EQ(st.query(0, 4), 3LL);
    st.update(1, 3, 1); // {3, 1, 1, 1, 3}
    EXPECT_EQ(st.query(0, 4), 1LL);
    EXPECT_EQ(st.query(0, 0), 3LL);
    EXPECT_EQ(st.query(4, 4), 3LL);
}

TEST_CASE(range_seg_tree_min_set_overlapping_updates)
{
    cp::RangeSegTree<cp::LongMinSetPolicy> st(5);
    st.update(0, 4, 5); // {5, 5, 5, 5, 5}
    st.update(2, 3, 2); // {5, 5, 2, 2, 5}
    EXPECT_EQ(st.query(0, 4), 2LL);
    EXPECT_EQ(st.query(0, 1), 5LL);
    st.update(1, 3, 8); // {5, 8, 8, 8, 5} - later set overwrites
    EXPECT_EQ(st.query(0, 4), 5LL);
    EXPECT_EQ(st.query(1, 3), 8LL);
}

TEST_CASE(range_seg_tree_sum_add_set_build_and_query)
{
    using L = cp::LongSumAddSetPolicy::L;
    cp::RangeSegTree<cp::LongSumAddSetPolicy> st({1, 2, 3, 4, 5});
    EXPECT_EQ(st.query(0, 4), 15LL);
    EXPECT_EQ(st.query(1, 3), 9LL);
    // pure add: {amount, nullopt}
    st.update(1, 3, L{2, std::nullopt}); // {1, 4, 5, 6, 5}
    EXPECT_EQ(st.query(0, 4), 21LL);
    EXPECT_EQ(st.query(1, 3), 15LL);
    EXPECT_EQ(st.query(0, 0), 1LL);
}

TEST_CASE(range_seg_tree_sum_add_set_range_update)
{
    using L = cp::LongSumAddSetPolicy::L;
    cp::RangeSegTree<cp::LongSumAddSetPolicy> st({1, 2, 3, 4, 5});
    // pure set: {0, value}
    st.update(1, 3, L{0, 10LL}); // {1, 10, 10, 10, 5}
    EXPECT_EQ(st.query(0, 4), 36LL);
    EXPECT_EQ(st.query(1, 3), 30LL);
    // set+add: {add, set} -> element = set+add
    st.update(2, 2, L{3, 5LL}); // element[2] = 5+3 = 8 -> {1, 10, 8, 10, 5}
    EXPECT_EQ(st.query(0, 4), 34LL);
    EXPECT_EQ(st.query(2, 2), 8LL);
}

TEST_CASE(range_seg_tree_sum_add_set_overlapping_updates)
{
    using L = cp::LongSumAddSetPolicy::L;
    cp::RangeSegTree<cp::LongSumAddSetPolicy> st(5);
    st.update(0, 4, L{0, 3LL}); // set all to 3: {3, 3, 3, 3, 3}
    EXPECT_EQ(st.query(0, 4), 15LL);
    st.update(1, 3, L{2, std::nullopt}); // add 2 to [1,3]: {3, 5, 5, 5, 3}
    EXPECT_EQ(st.query(0, 4), 21LL);
    st.update(0, 4, L{0, 10LL}); // set all to 10, discards prior adds: {10,10,10,10,10}
    EXPECT_EQ(st.query(0, 4), 50LL);
    st.update(0, 4, L{1, std::nullopt}); // add 1 to all: {11,11,11,11,11}
    EXPECT_EQ(st.query(0, 4), 55LL);
}

TEST_CASE(range_seg_tree_min_add_set_build_and_query)
{
    using L = cp::LongMinAddSetPolicy::L;
    cp::RangeSegTree<cp::LongMinAddSetPolicy> st({3, 1, 4, 1, 5});
    EXPECT_EQ(st.query(0, 4), 1LL);
    EXPECT_EQ(st.query(0, 2), 1LL);
    // pure add: {amount, nullopt}
    st.update(0, 4, L{2, std::nullopt}); // {5, 3, 6, 3, 7}
    EXPECT_EQ(st.query(0, 4), 3LL);
    EXPECT_EQ(st.query(0, 0), 5LL);
}

TEST_CASE(range_seg_tree_min_add_set_range_update)
{
    using L = cp::LongMinAddSetPolicy::L;
    cp::RangeSegTree<cp::LongMinAddSetPolicy> st({3, 1, 4, 1, 5});
    // pure set: {0, value}
    st.update(1, 3, L{0, 2LL}); // {3, 2, 2, 2, 5}
    EXPECT_EQ(st.query(0, 4), 2LL);
    EXPECT_EQ(st.query(0, 0), 3LL);
    // set+add: element = set+add
    st.update(2, 2, L{3, 5LL}); // element[2] = 5+3 = 8 -> {3, 2, 8, 2, 5}
    EXPECT_EQ(st.query(0, 4), 2LL);
    EXPECT_EQ(st.query(2, 2), 8LL);
}

TEST_CASE(range_seg_tree_min_add_set_overlapping_updates)
{
    using L = cp::LongMinAddSetPolicy::L;
    cp::RangeSegTree<cp::LongMinAddSetPolicy> st(5);
    st.update(0, 4, L{0, 5LL}); // set all to 5: {5, 5, 5, 5, 5}
    EXPECT_EQ(st.query(0, 4), 5LL);
    st.update(1, 3, L{-2, std::nullopt}); // add -2 to [1,3]: {5, 3, 3, 3, 5}
    EXPECT_EQ(st.query(0, 4), 3LL);
    EXPECT_EQ(st.query(0, 0), 5LL);
    st.update(0, 4, L{0, 10LL}); // set all to 10, discards prior: {10,10,10,10,10}
    EXPECT_EQ(st.query(0, 4), 10LL);
    st.update(2, 3, L{-3, std::nullopt}); // add -3 to [2,3]: {10,10,7,7,10}
    EXPECT_EQ(st.query(0, 4), 7LL);
    EXPECT_EQ(st.query(0, 1), 10LL);
}

TEST_CASE(range_seg_tree_assertions)
{
    cp::RangeSegTree<cp::LongSumAddPolicy> st(10);
    EXPECT_ABORT(st.update(-1, 5, 1));
    EXPECT_ABORT(st.update(0, 10, 1));
    EXPECT_ABORT(st.update(5, 3, 1));
    EXPECT_ABORT(st.query(-1, 5));
    EXPECT_ABORT(st.query(0, 10));
    EXPECT_ABORT(st.query(5, 3));
}
