// Chinese Remainder Theorem with
// generalization on non-coprime moduli
// https://www.programmersought.com/article/34462894954/

#include <bits/stdc++.h>
#define ll long long
using namespace std;

template<typename T>
T gcd(T a, T b) {
    return (a == 0) ? b : gcd(b % a, a);
}

template<typename T>
pair<T, pair<T,T> > reverseGCD(T a, T b) {
    // returns (g,(x,y))
    pair<T,pair<T,T>> ret;
    if (a == 0) {
        ret.first = b;
        ret.second.first = 0;
        ret.second.second = 1;
    } else {
        T g, x, y;
        pair<T,pair<T, T>> temp = reverseGCD(b%a, a);
        g = temp.first;
        x = temp.second.first;
        y = temp.second.second;
        ret.first = g;
        ret.second.first = y - (b/a)*x;
        ret.second.second = x;
    }
    return ret;
}

template<typename T>
T CRT(vector<T> n, vector<T> a) {
    // n periods, a is the remainder
    
    T Di = n[0], remainder = a[0];
    int k = n.size();

    for (int i = 1; i < k; ++i) {
        T di = n[i];
        T bi = a[i];
        auto rGCD = reverseGCD(Di, di);
        T x = rGCD.second.first, y = rGCD.second.second;

        T c = bi - remainder;
        
        if (c % rGCD.first) // indicates no solution
            return (T)(-1);

        T D = di / rGCD.first;
        remainder += Di * (((c / rGCD.first * x)%D+D)%D);

        Di *= D;
    }
 
    // represents the remainder of all zeros
    if (!remainder) {
        remainder = 1;
        for(int i = 0; i < k; ++i) {
            remainder = remainder * n[i] / gcd(remainder, n[i]);
        }
    }

    return remainder;
}
