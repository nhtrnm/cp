#pragma once
#include "cp/core/common.hpp"

namespace cp
{
// Array-backed segment tree supporting point updates and range queries.
//
// combine is a non-type template parameter (C++20): the callable instance is part of
// the type, not stored as a member. This differs from the typename approach where you
// pass the callable's type and construct an instance - with auto, calling combine(...)
// is a direct call to a statically-known function or operator, which the compiler can
// and should inline. The constraint: combine must be a structural type (captureless
// lambdas, function pointers, or empty structs like plus<ll>{}). Stateful or capturing
// callables are not supported. It must be associative with identity as its neutral
// element.
//
// Usage:
//   SegTree<ll, plus<ll>{}> st(n);
//   constexpr auto mn = [](ll a, ll b) { return min(a, b); };
//   SegTree<ll, mn, INF64> st(a);
template <typename T, auto combine, T identity = T{}>
struct SegTree
{
    int n;
    vector<T> tree;

    // O(n) time, O(n) space.
    SegTree(int size) : n(size), tree(4 * size, identity) {}

    // O(n) time, O(n) space - builds from initial values.
    SegTree(const vector<T> &a) : n(a.size()), tree(4 * a.size(), identity)
    {
        build(1, 0, n - 1, a);
    }

    // O(log n) time, O(log n) space - sets element at index idx to val.
    void update(int idx, T val)
    {
        assert(idx >= 0 && idx < n);
        update(1, 0, n - 1, idx, val);
    }

    // O(log n) time, O(log n) space - returns combine over [l, r].
    T query(int l, int r) const
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
        tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
    }

    void update(int v, int tl, int tr, int idx, T val)
    {
        if (tl == tr) {
            tree[v] = val;
            return;
        }
        int mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
        if (idx <= mid) {
            update(2 * v, tl, mid, idx, val);
        }
        else {
            update(2 * v + 1, mid + 1, tr, idx, val);
        }
        tree[v] = combine(tree[2 * v], tree[2 * v + 1]);
    }

    T query(int v, int tl, int tr, int l, int r) const
    {
        if (r < tl || tr < l) {
            return identity;
        }
        if (l <= tl && tr <= r) {
            return tree[v];
        }
        int mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
        return combine(query(2 * v, tl, mid, l, r),
                       query(2 * v + 1, mid + 1, tr, l, r));
    }
};

// Convenience alias for the common case of summing long long values.
using LongSumSegTree = SegTree<ll, plus<ll>{}>;
} // namespace cp
