// GCD/LCM Convolution
// Given seq a[i] and b[i], find c[i] such that
// c[k] = sum(a[i] * b[j]) where gcd(i, j) = k
// all MOD, N <= 10^6, a[i], b[i] <= MOD

// For LCM follow comments

#include <bits/stdc++.h>
#define ll long long
using namespace std;
constexpr int MOD = 998'244'353;

void zeta(vector<ll>& a) {
  int n = a.size() - 1;
  for (int i = 1; i <= n; ++i) { // reverse loop
    for (int j = 2; j <= n / i; ++j) {
      a[i] += a[i * j];             // swap(i, i*j)
      if (a[i] >= MOD) a[i] -= MOD; // swap(i, i*j)
    }
  }
}

void mobius(vector<ll>& a) {
  int n = a.size() - 1;
  for (int i = n; i >= 1; --i) { // reverse loop
    for (int j = 2; j <= n / i; ++j) {
      a[i] -= a[i * j];             // swap(i, i*j)
      if (a[i] < 0) a[i] += MOD;    // swap(i, i*j)
    }
  }
}

int main() {
  int n;
  scanf("%d", &n);

  vector<ll> a(n + 1), b(n + 1);
  for (int i = 1; i <= n; ++i) scanf("%lld", &a[i]);
  for (int i = 1; i <= n; ++i) scanf("%lld", &b[i]);

  zeta(a);
  zeta(b);
  vector<ll> c(n + 1);
  for (int i = 1; i <= n; ++i) c[i] = a[i] * b[i] % MOD;
  mobius(c);

  for (int i = 1; i <= n; ++i) {
    if (i > 1) printf(" ");
    printf("%lld", c[i]);
  }
  printf("\n");
  return 0;
}