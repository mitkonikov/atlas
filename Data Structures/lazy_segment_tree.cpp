struct LazySegmentTree {
    int N;
    
    struct Node {
        int ones = 0;
        int zeros = 0;
        bool lazy = 0;
        ll inversions1 = 0;
        ll inversions2 = 0;

        Node operator+(const Node &other) const {
            return {
                ones + other.ones,
                zeros + other.zeros,
                0,
                other.inversions1 + inversions1 + (ll)other.zeros * ones,
                other.inversions2 + inversions2 + (ll)other.ones * zeros
            };
        }
    };

    vector<Node> seg;
    LazySegmentTree(int n) {
        N = n;
        seg.resize(4 * N);
    }

    void push(int ind, int l, int r) {
        if (seg[ind].lazy == 0) return;
        swap(seg[ind].ones, seg[ind].zeros);
        swap(seg[ind].inversions1, seg[ind].inversions2);
        if (r - l != 1) {
            seg[2*ind+1].lazy ^= seg[ind].lazy;
            seg[2*ind+2].lazy ^= seg[ind].lazy;
        }
        seg[ind].lazy = 0;
    }

    void updateRange(int b, int e, int ind = 0, int l = 0, int r = -1) {
        if (r == -1) r = N;
        push(ind, l, r);
        if (b>=r || e<=l) return;
        if (b<=l && r<=e) {
            seg[ind].lazy ^= 1;
            push(ind, l, r);
            return;
        }
        int m = (l + r) / 2;
        updateRange(b, e, 2 * ind + 1, l, m);
        updateRange(b, e, 2 * ind + 2, m, r);
        seg[ind] = seg[2*ind+1] + seg[2*ind+2];
    }

    Node askRange(int b, int e, int ind = 0, int l = 0, int r = -1) {
        if (r == -1) r = N;
        push(ind, l, r);
        if (e<=l || b>=r) return { 0, 0, 0, 0, 0 };
        if (b<=l && r<=e) return seg[ind];
        int m = (l + r) / 2;
        auto res = askRange(b, e, 2 * ind + 1, l, m) + askRange(b, e, 2 * ind + 2, m, r);
        return res;
    }

    void put(int i, int what, int ind = 0, int l = 0, int r = -1) {
        if (r == -1) r = N;
        if (r - l == 1) {
            if (what) seg[ind].ones = 1;
            else seg[ind].zeros = 1;
            return;
        }
        int m = (l + r) / 2;
        if (i < m) put(i, what, 2 * ind + 1, l, m);
        else put(i, what, 2 * ind + 2, m, r);
        seg[ind] = seg[2*ind+1] + seg[2*ind+2];
    }
};
