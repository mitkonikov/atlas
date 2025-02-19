#define ll long long
void GospersHack(int n, int k, function<void(ll)> f) {
    ll set = (1LL << k) - 1;
    ll limit = (1LL << n);
    while (set < limit) {
        f(set);
        ll c = set & - set;
        ll r = set + c;
        set = (((r ^ set) >> 2) / c) | r;
    }
}