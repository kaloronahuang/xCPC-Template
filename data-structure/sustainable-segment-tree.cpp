// sustainable-segment-tree.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 1e5 + 2000;

struct node
{
    int sum, lson, rson;
} nodes[MAX_N << 4];

// the entries for the history versions;
int roots[MAX_N];

// the amount of the nodes in the system;
int tot;

// generate the tree and return the new created node id.
int build(int l, int r)
{
    int p = ++tot;
    if (l == r)
        return p;
    int mid = (l + r) >> 1;
    nodes[p].lson = build(l, mid), nodes[p].rson = build(mid + 1, r);
    return p;
}

// given the previous version, to generate a new version since the update;
int update(int qx, int l, int r, int pre, int val)
{
    int p = ++tot;
    // copy the info and update;
    nodes[p] = nodes[pre], nodes[p].sum += val;
    if (l == r)
        return p;
    int mid = (l + r) >> 1;
    // to edit son nodes;
    if (qx <= mid)
        nodes[p].lson = update(qx, l, mid, nodes[pre].lson, val);
    else
        nodes[p].rson = update(qx, mid + 1, r, nodes[pre].rson, val);
    return p;
}

// kth problem;
int query(int l, int r, int previous, int now, int k)
{
    int lsWeight = nodes[nodes[now].lson].sum - nodes[nodes[previous].lson].sum;
    if (l == r)
        return l;
    int mid = (l + r) >> 1;
    if (k <= lsWeight)
        return query(l, mid, nodes[previous].lson, nodes[now].lson, k);
    else
        return query(mid + 1, r, nodes[previous].rson, nodes[now].rson, k - lsWeight);
}