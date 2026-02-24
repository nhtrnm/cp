#include "../framework/test_framework.hpp"
#include "cp/math/mod_int.hpp"

TEST_CASE(mod_int_construction)
{
    cp::mint a(0);
    EXPECT_EQ(a.val, 0);
    cp::mint b(cp::MOD);
    EXPECT_EQ(b.val, 0);
    cp::mint c(-1);
    EXPECT_EQ(c.val, cp::MOD - 1);
    cp::mint d(cp::MOD + 5);
    EXPECT_EQ(d.val, 5);
}

TEST_CASE(mod_int_addition)
{
    cp::mint a(cp::MOD - 1);
    cp::mint b(1);
    EXPECT_EQ((a + b).val, 0);
    a += b;
    EXPECT_EQ(a.val, 0);
}

TEST_CASE(mod_int_subtraction)
{
    cp::mint a(0);
    cp::mint b(1);
    EXPECT_EQ((a - b).val, cp::MOD - 1);
    EXPECT_EQ((-b).val, cp::MOD - 1);
}

TEST_CASE(mod_int_multiplication)
{
    cp::mint a(cp::MOD / 2 + 1);
    cp::mint b(2);
    EXPECT_EQ((a * b).val, (long long)(cp::MOD / 2 + 1) * 2 % cp::MOD);
}

TEST_CASE(mod_int_pow)
{
    cp::mint a(2);
    EXPECT_EQ(a.pow(10).val, 1024);
    EXPECT_EQ(a.pow(0).val, 1);
}

TEST_CASE(mod_int_inv_and_division)
{
    cp::mint a(2);
    cp::mint inv_a = a.inv();
    EXPECT_EQ((a * inv_a).val, 1);
    cp::mint b(6), c(2);
    EXPECT_EQ((b / c).val, 3);
}
