#include "framework/test_framework.hpp"
#include <cstdlib>
#include <iostream>

int main()
{
    auto &tests = cp_test::Registry();
    int passed = 0;
    int failed = 0;

    for (auto &tc : tests) {
        try {
            tc.fn();
            std::cout << "[PASS] " << tc.name << "\n";
            ++passed;
        }
        catch (const std::exception &e) {
            std::cout << "[FAIL] " << tc.name << ": " << e.what() << "\n";
            ++failed;
        }
    }

    std::cout << "\n" << passed << "/" << (passed + failed) << " tests passed.\n";
    return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
