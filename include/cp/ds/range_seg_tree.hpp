#pragma once
#include "cp/core/common.hpp"

namespace cp
{
    // Array-backed segment tree supporting range-add updates and range-sum queries.
    //
    // Lazy propagation: full-overlap updates defer to lazy[v]; push_down is called on
    // partial overlaps in both update and query.
    //
    // For range-add, push_down in query is avoidable by accumulating ancestor lazy as a
    // parameter, but push_down is used here for consistency with other range updates
    // where that does not generalize.
    //
    // T must be wide enough to hold val * n without overflow (use ll, not int). T must
    // be an integer type because lazy[v] == T{} uses exact equality.
    template <typename T>
    struct SumRangeSegTree
    {
        int n;
        vector<T> tree;
        vector<T> lazy;

        // O(n) time, O(n) space
        SumRangeSegTree(int size) : n(size), tree(4 * size, T{}), lazy(4 * size, T{}) {}

        // O(n) time, O(n) space - builds from initial values
        SumRangeSegTree(const vector<T> &a)
            : n(a.size()), tree(4 * a.size(), T{}), lazy(4 * a.size(), T{})
        {
            build(1, 0, n - 1, a);
        }

        // O(log n) time, O(log n) space - adds val to every element in [l, r]
        void update(int l, int r, T val)
        {
            assert(l >= 0 && r < n && l <= r);
            update(1, 0, n - 1, l, r, val);
        }

        // O(log n) time, O(log n) space - returns sum of elements in [l, r].
        //
        // Note: has side effects - redistributes lazy values.
        T query(int l, int r)
        {
            assert(l >= 0 && r < n && l <= r);
            return query(1, 0, n - 1, l, r);
        }

    private:
        void build(int v, int tl, int tr, const vector<T> &a)
        {
            if (tl == tr) {
                tree[v] = a[tl];
                return;
            }
            int mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
            build(2 * v, tl, mid, a);
            build(2 * v + 1, mid + 1, tr, a);
            tree[v] = tree[2 * v] + tree[2 * v + 1];
        }

        // Pushes lazy[v] down to children and clears it.
        void push_down(int v, int tl, int tr)
        {
            if (lazy[v] == T{}) {
                return;
            }
            int mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
            tree[2 * v] += lazy[v] * (mid - tl + 1);
            lazy[2 * v] += lazy[v];
            tree[2 * v + 1] += lazy[v] * (tr - mid);
            lazy[2 * v + 1] += lazy[v];
            lazy[v] = T{};
        }

        void update(int v, int tl, int tr, int l, int r, T val)
        {
            if (r < tl || tr < l) {
                return;
            }
            if (l <= tl && tr <= r) {
                tree[v] += val * (tr - tl + 1);
                lazy[v] += val;
                return;
            }
            push_down(v, tl, tr);
            int mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
            update(2 * v, tl, mid, l, r, val);
            update(2 * v + 1, mid + 1, tr, l, r, val);
            tree[v] = tree[2 * v] + tree[2 * v + 1];
        }

        T query(int v, int tl, int tr, int l, int r)
        {
            if (r < tl || tr < l) {
                return T{};
            }
            if (l <= tl && tr <= r) {
                return tree[v];
            }
            push_down(v, tl, tr);
            int mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
            return query(2 * v, tl, mid, l, r) + query(2 * v + 1, mid + 1, tr, l, r);
        }
    };

    // Convenience alias for the common case of summing long long values.
    using LongSumRangeSegTree = SumRangeSegTree<ll>;
} // namespace cp
