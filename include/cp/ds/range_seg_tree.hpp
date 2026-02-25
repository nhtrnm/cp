#pragma once
#include "cp/core/common.hpp"

namespace cp
{
// Array-backed segment tree supporting range updates and range queries via lazy
// propagation.
//
// Behavior is fully determined by a Policy struct. A Policy must define:
//   using T = ...;                          // node/query value type
//   using L = ...;                          // lazy action type
//   static constexpr T tree_init = ...;     // initial value for tree nodes in empty tree
//   static constexpr L lazy_init = ...;     // identity lazy: merge(x, lazy_init) == x
//   static constexpr T query_oob = ...;     // combine identity: combine(x, query_oob) == x
//   static T combine(T left, T right);      // aggregate two child values
//   static T apply(T val, L lazy, int sz);  // apply lazy action to a node covering sz elements
//   static L merge(L existing, L incoming); // compose two lazies: incoming applied on top
//
// Constraints:
//   - combine must be associative with query_oob as its identity
//   - merge must correctly compose actions so that
//     apply(v, merge(a, b), sz) == apply(apply(v, a, sz), b, sz) for all v, a, b, sz
//   - lazy_init must be a no-op: apply(v, lazy_init, sz) == v for all v, sz
//
// Usage:
//   RangeSegTree<LongSumAddPolicy> st(n);        // empty tree of size n
//   RangeSegTree<LongSumAddPolicy> st({1,2,3});  // built from initial values
//   st.update(l, r, val);                        // apply lazy val to [l, r]
//   st.query(l, r);                              // return combine over [l, r]
//
// To define a custom policy:
//   struct MyPolicy {
//       using T = ll;
//       using L = ll;
//       static constexpr T tree_init = 0, query_oob = 0;
//       static constexpr L lazy_init = 0;
//       static T combine(T a, T b) { return a + b; }
//       static T apply(T v, L lz, int sz) { return v + lz * sz; }
//       static L merge(L e, L i) { return e + i; }
//   };
template <typename Policy>
struct RangeSegTree
{
    using T = Policy::T;
    using L = Policy::L;

    int n;
    vector<T> tree;
    vector<L> lazy;

    // O(n) time, O(n) space.
    RangeSegTree(int size)
        : n(size),
          tree(4 * size, Policy::tree_init),
          lazy(4 * size, Policy::lazy_init)
    {
        assert(size > 0);
    }

    // O(n) time, O(n) space - builds from initial values.
    RangeSegTree(const vector<T> &a)
        : n(a.size()),
          tree(4 * a.size(), Policy::tree_init),
          lazy(4 * a.size(), Policy::lazy_init)
    {
        assert(!a.empty());
        build(1, 0, n - 1, a);
    }

    // O(log n) time, O(log n) stack space - applies val to every element in [l, r].
    void update(int l, int r, L val)
    {
        assert(l >= 0 && r < n && l <= r);
        update(1, 0, n - 1, l, r, val);
    }

    // O(log n) time, O(log n) stack space - returns combined value of [l, r].
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
        int mid = tl + (tr - tl) / 2;
        build(2 * v, tl, mid, a);
        build(2 * v + 1, mid + 1, tr, a);
        tree[v] = Policy::combine(tree[2 * v], tree[2 * v + 1]);
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"

    void push_down(int v, int tl, int tr)
    {
        assert(tl < tr);
        if (lazy[v] == Policy::lazy_init) {
            return;
        }
        int mid = tl + (tr - tl) / 2;
        tree[2 * v] = Policy::apply(tree[2 * v], lazy[v], mid - tl + 1);
        lazy[2 * v] = Policy::merge(lazy[2 * v], lazy[v]);
        tree[2 * v + 1] = Policy::apply(tree[2 * v + 1], lazy[v], tr - mid);
        lazy[2 * v + 1] = Policy::merge(lazy[2 * v + 1], lazy[v]);
        lazy[v] = Policy::lazy_init;
    }

#pragma GCC diagnostic pop

    // Takes L (not T): a full-overlap update is identical to push_down - it applies a
    // lazy action onto the tree value and merges it into the node's lazy. Queries
    // produce T values; updates apply L actions.
    void update(int v, int tl, int tr, int l, int r, L val)
    {
        if (r < tl || tr < l) {
            return;
        }
        if (l <= tl && tr <= r) {
            tree[v] = Policy::apply(tree[v], val, tr - tl + 1);
            lazy[v] = Policy::merge(lazy[v], val);
            return;
        }
        push_down(v, tl, tr);
        int mid = tl + (tr - tl) / 2;
        update(2 * v, tl, mid, l, r, val);
        update(2 * v + 1, mid + 1, tr, l, r, val);
        tree[v] = Policy::combine(tree[2 * v], tree[2 * v + 1]);
    }

    T query(int v, int tl, int tr, int l, int r)
    {
        if (r < tl || tr < l) {
            return Policy::query_oob;
        }
        if (l <= tl && tr <= r) {
            return tree[v];
        }
        push_down(v, tl, tr);
        int mid = tl + (tr - tl) / 2;
        return Policy::combine(query(2 * v, tl, mid, l, r),
                               query(2 * v + 1, mid + 1, tr, l, r));
    }
};

struct LongSumAddPolicy
{
    using T = ll;
    using L = ll;

    static constexpr T tree_init = 0; // initial value for tree nodes in empty tree
    static constexpr L lazy_init = 0; // no-op lazy: merge(x, lazy_init) == x
    static constexpr T query_oob = 0; // combine identity: combine(x, query_oob) == x

    // Combines two node values.
    static T combine(T left_val, T right_val)
    {
        return left_val + right_val;
    }

    // Applies a lazy action to a node's value over size elements.
    static T apply(T val, L lazy, int size)
    {
        return val + lazy * size;
    }

    // Merges two lazies: incoming pushed on top of existing.
    static L merge(L existing, L incoming)
    {
        return existing + incoming;
    }
};

struct LongMinAddPolicy
{
    using T = ll;
    using L = ll;

    static constexpr T tree_init = 0;
    static constexpr L lazy_init = 0;
    static constexpr T query_oob = INF64;

    static T combine(T left_val, T right_val)
    {
        return min(left_val, right_val);
    }

    static T apply(T val, L lazy, int size)
    {
        (void)size;
        return val + lazy;
    }

    static L merge(L existing, L incoming)
    {
        return existing + incoming;
    }
};

struct LongSumSetPolicy
{
    using T = ll;
    using L = optional<ll>;

    static constexpr T tree_init = 0;
    static constexpr L lazy_init = nullopt;
    static constexpr T query_oob = 0;

    static T combine(T left_val, T right_val)
    {
        return left_val + right_val;
    }

    static T apply(T val, L lazy, int size)
    {
        return lazy.has_value() ? lazy.value() * size : val;
    }

    static L merge(L existing, L incoming)
    {
        return incoming.has_value() ? incoming : existing;
    }
};

struct LongMinSetPolicy
{
    using T = ll;
    using L = optional<ll>;

    static constexpr T tree_init = 0;
    static constexpr L lazy_init = nullopt;
    static constexpr T query_oob = INF64;

    static T combine(T left_val, T right_val)
    {
        return min(left_val, right_val);
    }

    static T apply(T val, L lazy, int size)
    {
        (void)size;
        return lazy.has_value() ? lazy.value() : val;
    }

    static L merge(L existing, L incoming)
    {
        return incoming.has_value() ? incoming : existing;
    }
};

struct LongSumAddSetPolicy
{
    using T = ll;
    using L = pair<ll, optional<ll>>;

    static constexpr T tree_init = 0;
    static constexpr L lazy_init = {0, nullopt};
    static constexpr T query_oob = 0;

    static T combine(T left_val, T right_val)
    {
        return left_val + right_val;
    }

    static T apply(T val, L lazy, int size)
    {
        return lazy.second.has_value() ? (lazy.first + lazy.second.value()) * size
                                       : val + lazy.first * size;
    }

    static L merge(L existing, L incoming)
    {
        return incoming.second.has_value()
                   ? incoming
                   : L{existing.first + incoming.first, existing.second};
    }
};

struct LongMinAddSetPolicy
{
    using T = ll;
    using L = pair<ll, optional<ll>>;

    static constexpr T tree_init = 0;
    static constexpr L lazy_init = {0, nullopt};
    static constexpr T query_oob = INF64;

    static T combine(T left_val, T right_val)
    {
        return min(left_val, right_val);
    }

    static T apply(T val, L lazy, int size)
    {
        (void)size;
        return lazy.second.has_value() ? lazy.first + lazy.second.value()
                                       : val + lazy.first;
    }

    static L merge(L existing, L incoming)
    {
        return incoming.second.has_value()
                   ? incoming
                   : L{existing.first + incoming.first, existing.second};
    }
};
} // namespace cp
