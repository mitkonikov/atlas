#include <bits/stdc++.h>
using namespace std;                 // A = 1 2 3
typedef long long ll;                // B = 1 2 3
typedef complex<double> cd;          // A * B = result = 1 4 10 12 9
                                     // (1 + 2x + 3x^2) * (1 + 2x + 3x^2)
const ll Max = 1e6 + 10;             //  1 + 2x + 3x^2 +
ll bound, logBound;                  //      2x + 4x^2 + 6x^3
const double pi = 4 * atan(1.0);     //           3x^2 + 6x^3 + 9x^2
cd root[Max], arrA[Max], arrB[Max];  //  1   4   10     12      9
ll perm[Max], prod[Max];

void fft(cd *arr) {
    for (ll i = 0; i < bound; i++) {
        if (i < perm[i]) {
            swap(arr[i], arr[perm[i]]);
        }
    }
    for (ll len = 1; len < bound; len *= 2) {
        for (ll pos = 0; pos < bound; pos += 2 * len) {
            for (ll i = 0; i < len; i++) {
                cd x = arr[pos + i],
                   y = arr[pos + i + len] * root[bound / len / 2 * i];
                arr[pos + i] = x + y;
                arr[pos + i + len] = x - y;
            }
        }
    }
}

void preCalc() {
    ll hb = -1;
    root[0] = 1;
    double angle = 2 * pi / bound;
    for (ll i = 1; i < bound; i++) {
        if ((i & (i - 1)) == 0) hb++;
        root[i] = cd(cos(angle * i), sin(angle * i));
        perm[i] = perm[i ^ (1 << hb)] + (1 << (logBound - hb - 1));
    }
}

void mult(vector<ll> &a, vector<ll> &b, vector<ll> &c) {
    logBound = 0;
    while ((1 << logBound) < a.size() || (1 << logBound) < b.size()) logBound++;
    logBound++;
    bound = (1 << logBound);
    preCalc();
    for (ll i = 0; i < a.size(); i++) {
        arrA[i] = cd(a[i], 0);
    }
    for (ll i = a.size(); i < bound; i++) {
        arrA[i] = cd(0, 0);
    }
    for (ll i = 0; i < b.size(); i++) {
        arrB[i] = cd(b[i], 0);
    }
    for (ll i = b.size(); i < bound; i++) {
        arrB[i] = cd(0, 0);
    }
    fft(arrA);
    fft(arrB);
    for (ll i = 0; i < bound; i++) {
        arrA[i] *= arrB[i];
    }
    fft(arrA);
    reverse(arrA + 1, arrA + bound);
    c.resize(bound);
    for (ll i = 0; i < bound; i++) {
        arrA[i] /= bound;
        ll temp =
            (arrA[i].real() > 0 ? arrA[i].real() + .5 : arrA[i].real() - .5);
        c[i] = temp;
    }
    while (c.size() && c.back() == 0) c.pop_back();
}

int main() {
    int n;
    cin >> n;
    vector<ll> a(n), b(n), result(2 * n + 100);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    mult(a, b, result);
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    return 0;
}