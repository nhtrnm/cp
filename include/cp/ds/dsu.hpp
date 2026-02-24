#pragma once
#include "cp/core/common.hpp"

namespace cp
{
    // DSU (disjoint set union) - tracks and merges disjoint sets with near-constant
    // time operations.
    // a(n) refers to the inverse Ackermann function, which grows so slowly it is
    // effectively O(1) for all practical input sizes.
    struct DSU
    {
        int n;
        vector<int> parents;
        vector<int> sizes;

        // O(n) time, O(n) space.
        DSU(int size) : n(size), parents(size), sizes(size, 1)
        {
            iota(parents.begin(), parents.end(), 0);
        }

        // Returns the root of u's set. O(a(n)) amortized.
        int find(int u)
        {
            if (u == parents[u]) {
                return u;
            }
            return parents[u] = find(parents[u]);
        }

        // Returns true if u and v are in the same set. O(a(n)) amortized.
        bool same(int u, int v)
        {
            return find(u) == find(v);
        }

        // Merges the sets containing u and v. O(a(n)) amortized.
        void merge(int u, int v)
        {
            u = find(u);
            v = find(v);
            if (u == v) {
                return;
            }
            if (sizes[u] < sizes[v]) {
                swap(u, v);
            }
            parents[v] = u;
            sizes[u] += sizes[v];
        }
    };
} // namespace cp
