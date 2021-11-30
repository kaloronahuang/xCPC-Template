# 上下界网络流

## 标定

考虑每条边的流量范围：$b(u, v) \leq f(u, v) \leq c(u, v)$。

## 无源汇上下界可行流

首先我们给这条边标入最大限制 $c(u, v) - b(u, v)$，并默认认为该边已经流入了 $b(u, v)$ 的流量，然后我们可以给每个点设置一个 $M_i$ 来进行流量搜集。

如果： 

- $M_i = 0$，那没啥问题。
- $M_i > 0$，说明下界要求流入的流量更多，所以我们从附加源点 $S$ 连到此点。
- $M_i < 0$，同理，说明下界要求流出的流量更多，所以我们专门开辟一个通道为此点流出至附加汇点 $T$。

代码：

```cpp
// LOJ115.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 220, MAX_E = 20400;

int n, m, head[MAX_N], current, di[MAX_N], start_pos, end_pos, dep[MAX_N], cur[MAX_N], idx[MAX_E], lowers[MAX_E];

struct edge
{
    int to, nxt, weight;
    bool tag;
} edges[MAX_E << 1];

void addpath(int src, int dst, int weight)
{
    edges[current].to = dst, edges[current].nxt = head[src];
    edges[current].weight = weight, head[src] = current++;
}

void addtube(int src, int dst, int weight)
{
    addpath(src, dst, weight);
    addpath(dst, src, 0);
}

bool bfs()
{
    memset(dep, 0, sizeof(dep));
    queue<int> q;
    q.push(start_pos), dep[start_pos] = 1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; i != -1; i = edges[i].nxt)
            if (dep[edges[i].to] == 0 && edges[i].weight > 0)
                dep[edges[i].to] = dep[u] + 1, q.push(edges[i].to);
    }
    return dep[end_pos] != 0;
}

int dfs(int u, int flow)
{
    if (flow == 0 || u == end_pos)
        return flow;
    for (int &i = cur[u]; i != -1; i = edges[i].nxt)
        if (dep[edges[i].to] == dep[u] + 1 && edges[i].weight > 0)
            if (int di = dfs(edges[i].to, min(flow, edges[i].weight)))
            {
                edges[i].weight -= di, edges[i ^ 1].weight += di;
                return di;
            }
    return 0;
}

int dinic()
{
    int ret = 0;
    while (bfs())
    {
        memcpy(cur, head, sizeof(head));
        while (int di = dfs(start_pos, 0x3f3f3f3f))
            ret += di;
    }
    return ret;
}

int main()
{
    memset(head, -1, sizeof(head));
    scanf("%d%d", &n, &m), start_pos = n + 1, end_pos = n + 2;
    for (int i = 1, u, v, lower, upper; i <= m; i++)
    {
        scanf("%d%d%d%d", &u, &v, &lower, &upper);
        idx[i] = current, addtube(u, v, upper - lower), di[u] -= lower, di[v] += lower, lowers[i] = lower;
    }
    for (int i = 1; i <= n; i++)
        if (di[i] < 0)
            addtube(i, end_pos, -di[i]);
        else if (di[i] > 0)
            addtube(start_pos, i, di[i]);
    dinic();
    bool flag = true;
    for (int i = head[start_pos]; i != -1; i = edges[i].nxt)
        flag &= (edges[i].weight == 0);
    if (flag)
    {
        puts("YES");
        for (int i = 1; i <= m; i++)
            printf("%d\n", edges[idx[i] ^ 1].weight + lowers[i]);
    }
    else
        puts("NO");
    return 0;
}
```

## 有源汇上下界可行流

把有源汇上下界网络 $G$ 中的 $T$ 向 $S$ 连接一条无限大的渠道，转换为无源汇上下界可行进行求解。

```cpp
addtube(end_pos, start_pos, 0x3f3f3f3f);
```

## 有源汇上下界最大流

其实有两种方法：

- 第一种：这个方法比较好理解，但是时间复杂度会比较大，大概是 $\Theta(\text{dinic}(n) \log_2 n)$。我们可以二分这个最大流，然后从 $T$ 到 $S$ 连一条下界为 $mid$、上界无穷大的的边，在有可行流的情况下保持最大流。
- 先按照「有源汇上下界可行流」的方法进行转换，先找一遍附加源点到附加汇点最大流，然后不管附加源点汇点，在原网络的参与网络上找 $S \to T$ 的最大流。不管附加源汇点其实可以直接把最后的那条 $T \to S$ 的边修改成 $0$ 边进行锁死，然后重新设置源汇点即可。复杂度为 $\Theta(\text{dinic}(n))$这个不是很好理解，可以看代码吧：

```cpp
// LOJ116.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 330, MAX_E = 1e5 + 200, INF = 0x3f3f3f3f;

typedef long long ll;

int n, m, gS, gT, start_pos, end_pos, head[MAX_N], current, dep[MAX_N], cur[MAX_N], di[MAX_N];

struct edge
{
    int to, nxt, weight;
} edges[MAX_E << 1];

void addpath(int src, int dst, int weight)
{
    edges[current].to = dst, edges[current].weight = weight;
    edges[current].nxt = head[src], head[src] = current++;
}

void addtube(int src, int dst, int weight)
{
    addpath(src, dst, weight);
    addpath(dst, src, 0);
}

bool bfs()
{
    memset(dep, 0, sizeof(dep));
    queue<int> q;
    q.push(start_pos), dep[start_pos] = 1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; i != -1; i = edges[i].nxt)
            if (dep[edges[i].to] == 0 && edges[i].weight > 0)
                dep[edges[i].to] = dep[u] + 1, q.push(edges[i].to);
    }
    return dep[end_pos] != 0;
}

int dfs(int u, int flow)
{
    if (u == end_pos || flow == 0)
        return flow;
    for (int &i = cur[u]; i != -1; i = edges[i].nxt)
        if (edges[i].weight > 0 && dep[edges[i].to] == dep[u] + 1)
            if (int di = dfs(edges[i].to, min(flow, edges[i].weight)))
            {
                edges[i].weight -= di, edges[i ^ 1].weight += di;
                return di;
            }
    return 0;
}

ll dinic()
{
    ll ret = 0;
    while (bfs())
    {
        memcpy(cur, head, sizeof(cur));
        while (int di = dfs(start_pos, 0x3f3f3f3f))
            ret += di;
    }
    return ret;
}

int main()
{
    memset(head, -1, sizeof(head));
    scanf("%d%d%d%d", &n, &m, &gS, &gT);
    start_pos = n + 1, end_pos = n + 2;
    for (int i = 1, u, v, lower, upper; i <= m; i++)
    {
        scanf("%d%d%d%d", &u, &v, &lower, &upper);
        addtube(u, v, upper - lower);
        di[u] -= lower, di[v] += lower;
    }
    for (int i = 1; i <= n; i++)
        if (di[i] < 0)
            addtube(i, end_pos, -di[i]);
        else
            addtube(start_pos, i, di[i]);
    addtube(gT, gS, INF);
    ll ret = dinic();
    bool flag = true;
    for (int i = head[start_pos]; i != -1; i = edges[i].nxt)
        flag &= edges[i].weight == 0;
    if (flag == false)
        puts("please go home to sleep"), exit(0);
    ret = edges[current - 1].weight;
    start_pos = gS, end_pos = gT;
    edges[current - 1].weight = edges[current - 2].weight = 0;
    ret += dinic(), printf("%lld\n", ret);
    return 0;
}
```

## 有源汇上下界最小流

这个就很学玄学了。首先还是有两种解法：

- 二分出流量，然后建图的时候限制下即可。
- 先按照「有源汇上下界可行流」的方法进行转换，先找一遍附加源点到附加汇点最大流，然后不管附加源点汇点，在原网络的参与网络上找 $T \to S$ 的最大流（注意这个关系变了）。不管附加源汇点其实可以直接把最后的那条 $T \to S$ 的边修改成 $0$ 边进行锁死，然后重新设置源汇点即可。复杂度为 $\Theta(\text{dinic}(n))$这个不是很好理解，可以看代码吧：

```cpp
// LOJ117.cpp
#include <bits/stdc++.h>

using namespace std;

const int MAX_N = 5e4 + 200, MAX_E = 3e5 + 200, INF = 0x3f3f3f3f;

typedef long long ll;

int n, m, gS, gT, start_pos, end_pos, head[MAX_N], current, dep[MAX_N], cur[MAX_N], di[MAX_N];

struct edge
{
    int to, nxt, weight;
} edges[MAX_E << 1];

void addpath(int src, int dst, int weight)
{
    edges[current].to = dst, edges[current].weight = weight;
    edges[current].nxt = head[src], head[src] = current++;
}

void addtube(int src, int dst, int weight)
{
    addpath(src, dst, weight);
    addpath(dst, src, 0);
}

bool bfs()
{
    memset(dep, 0, sizeof(dep));
    queue<int> q;
    q.push(start_pos), dep[start_pos] = 1;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; i != -1; i = edges[i].nxt)
            if (dep[edges[i].to] == 0 && edges[i].weight > 0)
                dep[edges[i].to] = dep[u] + 1, q.push(edges[i].to);
    }
    return dep[end_pos] != 0;
}

int dfs(int u, int flow)
{
    if (u == end_pos || flow == 0)
        return flow;
    for (int &i = cur[u]; i != -1; i = edges[i].nxt)
        if (edges[i].weight > 0 && dep[edges[i].to] == dep[u] + 1)
            if (int di = dfs(edges[i].to, min(flow, edges[i].weight)))
            {
                edges[i].weight -= di, edges[i ^ 1].weight += di;
                return di;
            }
    return 0;
}

ll dinic()
{
    ll ret = 0;
    while (bfs())
    {
        memcpy(cur, head, sizeof(cur));
        while (int di = dfs(start_pos, 0x3f3f3f3f))
            ret += di;
    }
    return ret;
}

int main()
{
    memset(head, -1, sizeof(head));
    scanf("%d%d%d%d", &n, &m, &gS, &gT);
    start_pos = n + 1, end_pos = n + 2;
    for (int i = 1, u, v, lower, upper; i <= m; i++)
    {
        scanf("%d%d%d%d", &u, &v, &lower, &upper);
        addtube(u, v, upper - lower);
        di[u] -= lower, di[v] += lower;
    }
    for (int i = 1; i <= n; i++)
        if (di[i] < 0)
            addtube(i, end_pos, -di[i]);
        else
            addtube(start_pos, i, di[i]);
    addtube(gT, gS, INF);
    ll ret = dinic();
    bool flag = true;
    for (int i = head[start_pos]; i != -1; i = edges[i].nxt)
        flag &= edges[i].weight == 0;
    if (flag == false)
        puts("please go home to sleep"), exit(0);
    ret = edges[current - 1].weight;
    start_pos = gT, end_pos = gS;
    edges[current - 1].weight = edges[current - 2].weight = 0;
    ret -= dinic(), printf("%lld\n", ret);
    return 0;
}
```

