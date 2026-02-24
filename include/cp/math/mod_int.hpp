#pragma once
#include "cp/core/common.hpp"

namespace cp
{
// Modular integer with compile-time modulus.
// M must be a prime for inv() to be correct (uses Fermat's little theorem).
template <int M>
struct ModInt
{
    int val;

    // Handles negative inputs correctly.
    ModInt(ll v = 0) : val(v % M)
    {
        if (val < 0) {
            val += M;
        }
    }

    ModInt &operator+=(const ModInt &o)
    {
        val += o.val;
        if (val >= M) {
            val -= M;
        }
        return *this;
    }

    ModInt operator+(const ModInt &o) const
    {
        return ModInt(*this) += o;
    }

    ModInt &operator-=(const ModInt &o)
    {
        val -= o.val;
        if (val < 0) {
            val += M;
        }
        return *this;
    }

    ModInt operator-(const ModInt &o) const
    {
        return ModInt(*this) -= o;
    }

    ModInt operator-() const
    {
        return ModInt(-val);
    }

    ModInt &operator*=(const ModInt &o)
    {
        val = (ll)val * o.val % M;
        return *this;
    }

    ModInt operator*(const ModInt &o) const
    {
        return ModInt(*this) *= o;
    }

    // O(log exp) time. exp must be >= 0.
    ModInt pow(ll exp) const
    {
        ModInt res = 1, base = *this;
        while (exp) {
            if (exp & 1) {
                res *= base;
            }
            base *= base;
            exp >>= 1;
        }
        return res;
    }

    // O(log M) time. M must be prime.
    ModInt inv() const
    {
        return pow(M - 2);
    }

    ModInt &operator/=(const ModInt &o)
    {
        return *this *= o.inv();
    }

    ModInt operator/(const ModInt &o) const
    {
        return ModInt(*this) /= o;
    }

    // C++20 auto-generates operator!= from operator==.
    bool operator==(const ModInt &o) const
    {
        return val == o.val;
    }

    friend ostream &operator<<(ostream &os, const ModInt &m)
    {
        return os << m.val;
    }

    friend istream &operator>>(istream &is, ModInt &m)
    {
        ll v;
        is >> v;
        m = ModInt(v);
        return is;
    }
};

// Convenience alias for the default modulus.
using mint = ModInt<MOD>;
} // namespace cp
