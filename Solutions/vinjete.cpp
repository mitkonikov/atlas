// "Vinjete" Task from COI2022
// Given a tree where each path adds an interval of numbers,
// calculate the interval cost from to root to each of the cities

#include <bits/stdc++.h>
#define ll long long
using namespace std;

struct edge {
    int u, v, l, r;
    void read() {
        cin >> u >> v >> l >> r;
        u--; v--; l--; r--;
    }
};

struct change {
    pair<int, int> interval; bool added = false; int effect = 0;
    int calc() { return effect = interval.second - interval.first; }
};

vector<vector<edge>> adj;
set<pair<int, int>> dsu;
vector<change> changes;
int current_ans = 0;
// [inclusive, exclusive).
int addInterval(int L, int R) {
    int change_in_ans = 0;
    if (L == R) return 0;
    auto it = dsu.lower_bound({L, R}), before = it;
    while (it != dsu.end() && it->first <= R) {
        R = max(R, it->second);
        changes.push_back({*it, false});
        change_in_ans -= changes.back().calc();
        before = it = dsu.erase(it);
    }
    if (it != dsu.begin() && (--it)->second >= L) {
        L = min(L, it->first);
        R = max(R, it->second);
        changes.push_back({*it, false});
        change_in_ans -= changes.back().calc();
        dsu.erase(it);
    }
    changes.push_back({{L, R}, true});
    change_in_ans += changes.back().calc();
    dsu.insert(before, {L, R});
    return change_in_ans;
}

int insert(int l, int r) {
    int sz = changes.size();
    current_ans += addInterval(l, r + 1);
    return sz;
}

void rollback(int size) {
    while (changes.size() > size) {
        change c = changes.back();
        changes.pop_back();
        if (c.added) {
            current_ans -= c.effect;
            dsu.erase(c.interval);
        } else {
            current_ans += c.effect;
            dsu.insert(c.interval);
        }
    }
}

vector<int> ans;
void dfs(int u, int p) {
    ans[u] = current_ans;
    for (edge e : adj[u]) {
        if (e.v == p) continue;
        int size = insert(e.l, e.r);
        dfs(e.v, u);
        rollback(size);
    }
}

int main() {
    int n, m;
    cin >> n >> m; adj.resize(n); ans.resize(n, 0); changes.reserve(3e5);
    for (int i = 0; i < n - 1; i++) {
        edge e; e.read();
        adj[e.u].push_back(e); swap(e.u, e.v);
        adj[e.u].push_back(e);
    }
    dfs(0, -1);
    for (int i = 1; i < n; i++) cout << ans[i] << endl; return 0;
}