// sam.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 1e6 + 200;

struct node
{
    int dep, link, ch[26], pos;
} nodes[MAX_N << 1];

int ptot = 1, last_ptr = 1, bucket[MAX_N << 1], rnk[MAX_N << 1];
vector<int> G[MAX_N << 1];

void insert(int c, int idx)
{
    int pre = last_ptr, p = last_ptr = ++ptot;
    nodes[p].dep = nodes[pre].dep + 1, nodes[p].pos = idx;
    while (pre && nodes[pre].ch[c] == 0)
        nodes[pre].ch[c] = p, pre = nodes[pre].link;
    if (pre == 0)
        nodes[p].link = 1;
    else
    {
        int q = nodes[pre].ch[c];
        if (nodes[q].dep == nodes[pre].dep + 1)
            nodes[p].link = q;
        else
        {
            int clone = ++ptot;
            nodes[clone] = nodes[q], nodes[clone].dep = nodes[pre].dep + 1;
            nodes[p].link = nodes[q].link = clone;
            while (pre && nodes[pre].ch[c] == q)
                nodes[pre].ch[c] = clone, pre = nodes[pre].link;
        }
    }
}

void radixSort()
{
    for (int i = 1; i <= ptot; i++)
        bucket[nodes[i].dep]++;
    for (int i = 1; i <= ptot; i++)
        bucket[i] += bucket[i - 1];
    for (int i = 1; i <= ptot; i++)
        rnk[bucket[nodes[i].dep]--] = i;
    for (int i = 2; i <= ptot; i++)
        G[nodes[rnk[i]].link].push_back(rnk[i]);
}