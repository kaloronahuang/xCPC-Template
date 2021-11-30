// tarjan.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 1e5 + 200;

int head[MAX_N], current, n, m, ncnt, vi[MAX_N], value[MAX_N], aff[MAX_N];
int dfn[MAX_N], low[MAX_N], ptot, stk[MAX_N], tail, dp[MAX_N];
bool inst[MAX_N], vis[MAX_N];
vector<int> G[MAX_N];

struct edge
{
    int to, nxt;
} edges[MAX_N << 1];

void addpath(int src, int dst)
{
    edges[current].to = dst, edges[current].nxt = head[src];
    head[src] = current++;
}

void tarjan(int u)
{
    dfn[u] = low[u] = ++ptot, inst[u] = true, stk[++tail] = u;
    for (int i = head[u]; i != -1; i = edges[i].nxt)
        if (dfn[edges[i].to] == 0)
            tarjan(edges[i].to), low[u] = min(low[u], low[edges[i].to]);
        else if (inst[edges[i].to])
            low[u] = min(low[u], dfn[edges[i].to]);
    if (dfn[u] == low[u])
    {
        ncnt++;
        do
        {
            aff[stk[tail]] = ncnt, inst[stk[tail]] = false;
            value[ncnt] += vi[stk[tail]];
        } while (stk[tail--] != u);
    }
}

void dfs(int u, int fa)
{
    if (vis[u])
        return;
    dp[u] = value[u], vis[u] = true;
    for (int i = 0, siz = G[u].size(); i < siz; i++)
        if (G[u][i] != fa)
            dfs(G[u][i], u), dp[u] = max(dp[u], dp[G[u][i]] + value[u]);
}

int main()
{
    memset(head, -1, sizeof(head));
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%d", &vi[i]);
    for (int i = 1, u, v; i <= m; i++)
        scanf("%d%d", &u, &v), addpath(u, v);
    for (int i = 1; i <= n; i++)
        if (dfn[i] == 0)
            tarjan(i);
    for (int u = 1; u <= n; u++)
        for (int i = head[u]; i != -1; i = edges[i].nxt)
            if (aff[u] != aff[edges[i].to])
                G[aff[u]].push_back(aff[edges[i].to]);
    int ans = 0;
    for (int i = 1; i <= ncnt; i++)
    {
        dfs(i, 0);
        ans = max(ans, dp[i]);
    }
    printf("%d", ans);
    return 0;
}