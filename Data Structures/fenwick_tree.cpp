struct FenwickTree {
    vector<ll> fwt;
    int LOGN = 20;

    FenwickTree(int n) {
        fwt.resize(n, 0);
        LOGN = log2(n) + 1;
    }

    void add(int ind, ll val = 1) {
        for (ind++; ind < fwt.size(); ind+=ind&-ind)
            fwt[ind]+=val;
    }

    ll query(int ind) {
        ll s = 0;
        for (ind++; ind > 0; ind-=ind&-ind)
            s += fwt[ind];
        return s;
    }

    ll lower_bound(int v) {
        v++;
        ll sum = 0;
        int pos = 0;
        for (int i = LOGN; i >= 0; i--) {
            if (pos + (1 << i) < fwt.size() &&
                sum + fwt[pos + (1 << i)] < v) {
                    sum += fwt[pos + (1 << i)];
                    pos += (1 << i);
            }
        }
        return pos - 1;
    }
};