#pragma once
#include "cp/core/common.hpp"

namespace cp
{
    // Fenwick tree (Binary Indexed Tree) - 0-indexed.
    // Reference: https://cp-algorithms.com/data_structures/fenwick.html
    template <typename T>
    struct Fenwick
    {
        int n;
        vector<T> tree;

        // O(n) time, O(n) space.
        Fenwick(int size) : n(size), tree(size) {}

        // O(n log n) time - builds from an existing array.
        Fenwick(const vector<T> &a) : n(a.size()), tree(a.size())
        {
            for (int i = 0; i < n; i++) {
                add(i, a[i]);
            }
        }

        // Adds delta to index idx. O(log n) time.
        void add(int idx, T delta)
        {
            for (; idx < n; idx = idx | (idx + 1)) {
                tree[idx] += delta;
            }
        }

        // Returns the prefix sum [0, r]. O(log n) time.
        T query(int r) const
        {
            T result{};
            for (; r >= 0; r = (r & (r + 1)) - 1) {
                result += tree[r];
            }
            return result;
        }

        // Returns the range sum [l, r]. O(log n) time.
        T query(int l, int r) const
        {
            return query(r) - query(l - 1);
        }
    };
} // namespace cp
