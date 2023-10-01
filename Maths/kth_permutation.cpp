#include <bits/stdc++.h>
using namespace std;

struct KthPermutation {
    #define ull unsigned long long
    ull factorial[21];
    
    KthPermutation() {
        factorial[0] = 1;
        for (int i = 1; i <= 20; i++) {
            factorial[i] = factorial[i-1] * i;
        }
    }

    // O(n^2)
    vector<int> kth(int n, ull k) {
        vector<int> v(n);
        iota(v.begin(), v.end(), 1);
        vector<int> result;

        do {
            ull count = factorial[v.size() - 1];
            ull selected = (k - 1) / count;
            result.push_back(v[selected]);
            v.erase(v.begin() + selected);
            k -= (count * selected);
        } while (!v.empty());

        return result;
    }
};

int main() {
    KthPermutation util;
    
    for (int i = 1; i <= 24; ++i) {
        vector<int> permutation = util.kth(4, i);
        for (auto element: permutation) {
            cout << element << " ";
        } cout << endl;
    }
}