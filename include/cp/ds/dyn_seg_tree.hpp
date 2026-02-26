#pragma once
#include "cp/core/common.hpp"

namespace cp
{
// Pointer-based lazy segment tree over an arbitrary range [lo, hi] supporting range-add
// updates and range-sum queries.
//
// Nodes are allocated on demand, making it efficient for large but sparse ranges (e.g.
// coordinates up to 1e9 with few updates). Memory is not freed on destruction, meant
// for single-use CP problems. Total space: O(q * log(hi - lo)) across q operations.
//
// T must be wide enough to hold lazy * (hi - lo + 1) without overflow. For ranges up to
// 1e9 with values up to 1e9, use ll (not int).
//
// To adapt for other operations (e.g. range-set, range-min), modify push_down and query
// to use the appropriate lazy-application and combine logic.
template <typename T>
struct DynSegTree
{
    struct Node
    {
        T val;
        T lazy;
        Node *left;
        Node *right;

        Node() : val(T{}), lazy(T{}), left(nullptr), right(nullptr) {}
    };

    ll lo, hi;
    Node *root;

    // O(1) time, O(1) space.
    DynSegTree(ll size) : lo(0), hi(size - 1), root(new Node())
    {
        assert(size > 0);
    }

    // O(1) time, O(1) space.
    DynSegTree(ll l, ll r) : lo(l), hi(r), root(new Node())
    {
        assert(l <= r);
    }

    // Adds val to every element in [l, r]. O(log(hi - lo)) time, O(log(hi - lo)) space.
    void update(ll l, ll r, T val)
    {
        assert(l >= lo && r <= hi && l <= r);
        update(root, lo, hi, l, r, val);
    }

    // Returns sum of elements in [l, r]. O(log(hi - lo)) time, O(1) space.
    T query(ll l, ll r) const
    {
        assert(l >= lo && r <= hi && l <= r);
        return query(root, lo, hi, l, r);
    }

private:
    // Pushes lazy down to children, creating them if needed.
    void push_down(Node *node, ll tl, ll tr)
    {
        assert(tl < tr); // cannot apply to leaves
        if (!node->left) {
            node->left = new Node();
        }
        if (!node->right) {
            node->right = new Node();
        }
        if (node->lazy == T{}) {
            return;
        }
        ll mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
        node->left->val += node->lazy * (mid - tl + 1);
        node->left->lazy += node->lazy;
        node->right->val += node->lazy * (tr - mid);
        node->right->lazy += node->lazy;
        node->lazy = T{};
    }

    void update(Node *node, ll tl, ll tr, ll l, ll r, T val)
    {
        if (r < tl || tr < l) {
            return;
        }
        if (l <= tl && tr <= r) {
            node->val += val * (tr - tl + 1);
            node->lazy += val;
            return;
        }
        push_down(node, tl, tr);
        ll mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
        update(node->left, tl, mid, l, r, val);
        update(node->right, mid + 1, tr, l, r, val);
        node->val = node->left->val + node->right->val;
    }

    // Returns T{} for null nodes (unvisited subtrees are implicitly zero).
    //
    // push_down is not called - it would allocate nodes inside a read-only operation.
    // Instead, at each partial overlap, node->lazy * overlap([l,r], [tl,tr]) is added
    // to correct for the pending lazy. This logically adds complexity over push_down
    // (which pushes all info to children to be summed directly), but keeps query const.
    //
    // Null children return T{} safely since ancestor lazies are handled by the overlap
    // term above them.
    T query(const Node *node, ll tl, ll tr, ll l, ll r) const
    {
        if (!node || r < tl || tr < l) {
            return T{};
        }
        if (l <= tl && tr <= r) {
            return node->val;
        }
        ll mid = tl + (tr - tl) / 2; // avoids overflow vs (tl + tr) / 2
        return query(node->left, tl, mid, l, r) +
               query(node->right, mid + 1, tr, l, r) +
               node->lazy * (min(tr, r) - max(tl, l) + 1);
    }
};

// Convenience alias for the common case of summing long long values.
using LongDynSegTree = DynSegTree<ll>;
} // namespace cp
