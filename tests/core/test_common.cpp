#include "../framework/test_framework.hpp"
#include "cp/core/common.hpp"

TEST_CASE(typedefs_are_correct_sizes)
{
    EXPECT_EQ(sizeof(cp::ll), 8u);
    EXPECT_EQ(sizeof(cp::ull), 8u);
}

TEST_CASE(inf_constants_are_positive)
{
    EXPECT_TRUE(cp::INF > 0);
    EXPECT_TRUE(cp::INF64 > 0);
}
