// fft.cpp
#include <bits/stdc++.h>

using namespace std;

typedef complex<double> cd;

const int MAX_N = 1e5 + 200;
const double pi = acos(-1.0);

int poly_bit, poly_siz, rev[MAX_N];

void fft_initialize()
{
    for (int i = 0; i < poly_siz; i++)
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (poly_bit - 1));
}

void fft(cd *arr, int dft)
{
    for (int i = 0; i < poly_siz; i++)
        if (i < rev[i])
            swap(arr[i], arr[rev[i]]);
    for (int step = 1; step < poly_siz; step <<= 1)
    {
        cd omega_n = exp(cd(0, dft * pi / step));
        for (int j = 0; j < poly_siz; j += (step << 1))
        {
            cd omega_nk(1, 0);
            for (int k = j; k < j + step; k++, omega_nk *= omega_n)
            {
                cd t = arr[k + step] * omega_nk;
                arr[k + step] = arr[k] - t;
                arr[k] += t;
            }
        }
    }
    if (dft == -1)
        for (int i = 0; i < poly_siz; i++)
            arr[i] /= poly_siz;
}