#pragma once
#include <cmath>
#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace cp_test
{
    struct TestCase
    {
        std::string name;
        std::function<void()> fn;
    };

    inline std::vector<TestCase> &Registry()
    {
        static std::vector<TestCase> reg;
        return reg;
    }

    struct TestRegistrar
    {
        TestRegistrar(const std::string &name, std::function<void()> fn)
        {
            Registry().push_back({name, fn});
        }
    };

#define TEST_CASE(name)                                                                \
    static void _test_##name();                                                        \
    static ::cp_test::TestRegistrar _reg_##name(#name, _test_##name);                  \
    static void _test_##name()

#define EXPECT_TRUE(expr)                                                              \
    do {                                                                               \
        if (!(expr)) {                                                                 \
            throw std::runtime_error(                                                  \
                std::string("EXPECT_TRUE failed: " #expr " at " __FILE__ ":") +        \
                std::to_string(__LINE__));                                             \
        }                                                                              \
    }                                                                                  \
    while (0)

#define EXPECT_EQ(a, b)                                                                \
    do {                                                                               \
        if (!((a) == (b))) {                                                           \
            throw std::runtime_error(                                                  \
                std::string("EXPECT_EQ failed: " #a " != " #b " at " __FILE__ ":") +   \
                std::to_string(__LINE__));                                             \
        }                                                                              \
    }                                                                                  \
    while (0)

#define EXPECT_NEAR(a, b, eps)                                                         \
    do {                                                                               \
        if (!(std::abs((a) - (b)) <= (eps))) {                                         \
            throw std::runtime_error(std::string("EXPECT_NEAR failed: |" #a " - " #b   \
                                                 "| > " #eps " at " __FILE__ ":") +    \
                                     std::to_string(__LINE__));                        \
        }                                                                              \
    }                                                                                  \
    while (0)

#define FAIL(msg)                                                                      \
    do {                                                                               \
        throw std::runtime_error(std::string("FAIL: " msg " at " __FILE__ ":") +       \
                                 std::to_string(__LINE__));                            \
    }                                                                                  \
    while (0)

} // namespace cp_test
