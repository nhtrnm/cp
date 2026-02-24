#include "framework/test_framework.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;

int main()
{
    auto &tests = cp_test::Registry();
    int passed = 0;
    int failed = 0;

    for (auto &tc : tests) {
        try {
            tc.fn();
            cout << "[PASS] " << tc.name << "\n";
            ++passed;
        }
        catch (const exception &e) {
            cout << "[FAIL] " << tc.name << ": " << e.what() << "\n";
            ++failed;
        }
    }

    cout << "\n" << passed << "/" << (passed + failed) << " tests passed.\n";
    return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
