// fenwick-tree.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 1e6 + 200;

int n, tree[MAX_N];

inline int lowbit(int x) { return x & -x; }
 
void update(int x, int d)
{
    for (; x <= n; x += lowbit(x))
        tree[x] += d;
}

int query(int x, int ret = 0)
{
	for (; x; x -= lowbit(x))
		ret += tree[x];
	return ret;
}
 
int kth(int k)
{
    int ret = 0;
    for (int i = 22; i >= 0; i--)
    {
        ret += (1 << i);
        if (ret >= n || k <= tree[ret])
            ret -= (1 << i);
        else
            k -= tree[ret];
    }
    return ret + 1;
}