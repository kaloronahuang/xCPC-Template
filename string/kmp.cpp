// kmp.cpp
// Complexity: O(n)
#include <bits/stdc++.h>

const int MAX_N = 1e5 + 200;

int nxt[MAX_N];

void getNxt(char *str)
{
    nxt[1] = 0;
    for (int i = 2, j = 0; str[i]; i++)
    {
        while (j > 0 && str[i] != str[j + 1])
            j = nxt[j];
        if (str[i] == str[j + 1])
            j++;
        nxt[i] = j;
    }
}

void kmp(char *S, char *T)
{
    int n = strlen(S + 1), m = strlen(T + 1);
    for (int i = 1, j = 1; i <= n; i++)
    {
        while (j > 0 && T[j + 1] != S[i])
            j = nxt[j];
        if (T[j + 1] == S[i])
        {
            j++;
            if (j == m)
                printf("%d ", i - m + 1), j = nxt[j];
        }
    }
}