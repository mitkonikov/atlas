/*
        Finds the smallest linear recurrence for the sequence given as input.
    For example, for the sequence
        0 1 1 2 3 5 8 13
        The recursion will be: f(x) = 1 * f(x-1) + 1 * f(x-2)
*/

#include <bits/stdc++.h>
#define ll long long
using namespace std;

struct BerlekampMassey {
    const int M = 1e9 + 7;
    ll fast_pow(ll a, ll b) {
        ll ans = 1;

        while (b) {
            if (b & 1) ans = ans * a % M;
            b >>= 1;
            a = a * a % M;
        }
        return ans;
    }
    inline ll inv(ll x) { return fast_pow(x, M - 2); }
    // 'vec' is the array with the first vec.size() recurrence numbers
    // 'cur' is the array of multiplied coefficients,
    //  and must be interpreted as
    //  vec[i] = cur [0] * vec [i-1] + cur [1] * vec [i-2] + ...
    vector<ll> solve(vector<ll> vec) {
        const int n = vec.size();

        // S[i] = calculated recurrence for the prefix i,
        // which evaluates to 0 for all indices from 0 to i
        // and evaluates to 1 for i+1
        vector<vector<ll> > S(n, vector<ll>());

        // current recurrence
        vector<ll> cur = {0};
        S[0] = {1};
        for (int i = 1; i < n; ++i) {
            ll res = 0;
            assert((int)cur.size() <= i);
            for (int j = 0; j < cur.size(); ++j) {
                res = (res + cur[j] * vec[i - 1 - j] % M) % M;
            }

            if (vec[i] == res) continue;    // it worked out
            ll v = (res - vec[i] + M) % M;  // v such that vec[i] + v = res

            // Calculate S[i]
            S[i].push_back(M - 1 * inv(v) % M);
            for (ll x : cur) S[i].push_back(x * inv(v) % M);

            // Recalculate cur
            int k = 0;
            for (int j = 0; j < i; ++j) {
                if (S[j].size() == 0) continue;
                if (S[j].size() + i - 1 - j <= S[k].size() + i - 1 - k) k = j;
            }

            vector<ll> aux(max(int(S[k].size()) + i - 1 - k, (int)cur.size()));
            for (int j = 0; j < aux.size(); ++j) {
                ll x = j < (i - 1 - k) ? 0 : S[k][j - (i - 1 - k)];
                aux[j] = ((vec[i] - res + M) * x % M) +
                         (j < cur.size() ? cur[j] : 0);
                aux[j] %= M;
            }

            cur = aux;
        }

        return cur;
    }
};

int main() {
    int n;
    cin >> n;
    vector<ll> v(n);
    for (int i = 0; i < n; i++) cin >> v[i];
    BerlekampMassey bm;              // for 0 1 3 10 33 returns 3 1 0
    vector<ll> coeff = bm.solve(v);  //   f(n) = 3 * f(n-1) + f(n-2)
    for (int i = 0; i < coeff.size(); i++) cout << coeff[i] << " ";
    cout << endl;
    return 0;
}