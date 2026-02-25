#pragma once
#include "cp/core/common.hpp"

namespace cp
{
// Fenwick tree (Binary Indexed Tree) - 0-indexed, supports point-add updates and
// range-sum queries.
//
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

// Fenwick tree over an arbitrary index range [l, r].
//
// Indices are shifted internally to [0, r-l] and delegated to Fenwick<T>.
template <typename T>
struct OffsetFenwick
{
    int lo, hi;
    Fenwick<T> fw;

    // O(n) time, O(n) space. n = hi - lo + 1.
    OffsetFenwick(int l, int r) : lo(l), hi(r), fw(r - l + 1) {}

    // O(n log n) time - builds from an existing array over [l, l + a.size() - 1].
    OffsetFenwick(int l, const vector<T> &a) : lo(l), hi(l + a.size() - 1), fw(a) {}

    // Adds delta to index idx. O(log n) time.
    void add(int idx, T delta)
    {
        assert(idx >= lo && idx <= hi);
        fw.add(idx - lo, delta);
    }

    // Returns the prefix sum [lo, idx]. O(log n) time.
    T query(int idx) const
    {
        assert(idx >= lo && idx <= hi);
        return fw.query(idx - lo);
    }

    // Returns the range sum [ql, qr]. O(log n) time.
    T query(int ql, int qr) const
    {
        assert(ql >= lo && qr <= hi && ql <= qr);
        return fw.query(ql - lo, qr - lo);
    }
};
} // namespace cp
