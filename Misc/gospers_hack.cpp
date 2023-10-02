// 
//  Iterate through the bit masks by increasing number of ON bits.
//  For every C(N, K) bits the function f() is called.
// 

#define ll long long
void GospersHack(int n, int k, function<void(ll)> f) {
    ll set = (1 << k) - 1;
    ll limit = (1 << n);
    while (set < limit) {
        f(set);
        ll c = set & - set;
        ll r = set + c;
        set = (((r ^ set) >> 2) / c) | r;
    }
}