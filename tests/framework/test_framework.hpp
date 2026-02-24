// Test framework - lightweight registration-based test runner.
//
// How it works:
//   TEST_CASE(name) defines a static function and registers it via a static
//   TestRegistrar, whose constructor runs before main() and pushes the test into a
//   global registry. test_main.cpp iterates the registry and runs each test, catching
//   exceptions to report PASS/FAIL.
//
// How to add a test:
//   1. Create tests/<category>/test_<module>.cpp
//   2. Include "../framework/test_framework.hpp" and the header under test
//   3. Write TEST_CASE blocks using EXPECT_* macros
//   4. Run make test - the Makefile auto-discovers all .cpp files under tests/
//
// Available assertions:
//   EXPECT_TRUE(expr)       - expr must be true
//   EXPECT_FALSE(expr)      - expr must be false
//   EXPECT_EQ(a, b)         - a == b
//   EXPECT_NEAR(a, b, eps)  - |a - b| <= eps
//   EXPECT_ABORT(expr)      - expr must call abort() (for testing assert())
//   FAIL(msg)               - unconditionally fail with message
#pragma once
#include <cmath>
#include <csignal>
#include <functional>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
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

// All EXPECT_* macros use do { ... } while (0) to behave as a single statement.
// Without it, multi-statement macros break in if/else without braces, e.g.:
//   if (cond)
//     EXPECT_TRUE(x);  // expands to two statements - only the first is in the if
//   else ...           // now dangles, attaching to the wrong if
// do { ... } while (0) is the only construct that:
//   1. wraps multiple statements into one
//   2. requires a semicolon after the macro call (EXPECT_TRUE(x);)
//   3. compiles away to nothing at runtime since while (0) is always false
#define EXPECT_TRUE(expr)                                                              \
    do {                                                                               \
        if (!(expr)) {                                                                 \
            throw std::runtime_error(                                                  \
                std::string("EXPECT_TRUE failed: " #expr " at " __FILE__ ":") +        \
                std::to_string(__LINE__));                                             \
        }                                                                              \
    }                                                                                  \
    while (0)

#define EXPECT_FALSE(expr)                                                             \
    do {                                                                               \
        if (expr) {                                                                    \
            throw std::runtime_error(                                                  \
                std::string("EXPECT_FALSE failed: " #expr " at " __FILE__ ":") +       \
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

// Forks a child process, runs expr, and expects it to abort (SIGABRT).
// Used to test assert() calls.
#define EXPECT_ABORT(expr)                                                             \
    do {                                                                               \
        pid_t pid = fork();                                                            \
        if (pid == 0) {                                                                \
            /* suppress assert() error message printed by child */                     \
            close(STDERR_FILENO);                                                      \
            expr;                                                                      \
            _exit(0);                                                                  \
        }                                                                              \
        int status;                                                                    \
        waitpid(pid, &status, 0);                                                      \
        if (!WIFSIGNALED(status) || WTERMSIG(status) != SIGABRT) {                     \
            throw std::runtime_error(                                                  \
                std::string("EXPECT_ABORT failed: " #expr " at " __FILE__ ":") +       \
                std::to_string(__LINE__));                                             \
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
