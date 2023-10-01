struct Query {
    int id;
    int l, r;

    bool operator<(const Query &rhs) const {
        if (l / MAGIC == rhs.l / MAGIC) return r < rhs.r;
        return (l / MAGIC < rhs.l / MAGIC);
    }
};

vector<int> ans_vec(q);
int cur_l = 0, cur_r = -1;
for (int i = 0; i < q; i++) {
    while (cur_l > queries[i].l) {
        cur_l--;
        add(cur_l);
    }

    while (cur_r < queries[i].r) {
        cur_r++;
        add(cur_r);
    }

    while (cur_l < queries[i].l) {
        remove(cur_l);
        cur_l++;
    }

    while (cur_r > queries[i].r) {
        remove(cur_r);
        cur_r--;
    }

    ans_vec[queries[i].id] = ans;
}
