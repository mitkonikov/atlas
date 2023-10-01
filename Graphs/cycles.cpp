// How many nodes end up in a cycle in a DCG?

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<bool> recStack;
vector<bool> visited;
vector<bool> is_cycle;

bool dfs(int u) {
    recStack[u] = visited[u] = true;
    for (auto v: adj[u]) {
        if (is_cycle[v] || (!visited[v] && dfs(v)) || recStack[v]) {
            // If you don't need to reset the recursion stack
            // you won't need to check is_cycle[v]
            recStack[u] = false;
            return is_cycle[v] = true;
        }
    }

    recStack[u] = false;
    return false;
}

int main() {
    int n, m; cin >> n >> m;
    adj.resize(n);
    visited.resize(n, false);
    recStack.resize(n, false);
    is_cycle.resize(n, false);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v; u--; v--;
        adj[u].push_back(v);
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if (dfs(i)) {
                is_cycle[i] = true;
            }
        }
    }

    int count = accumulate(is_cycle.begin(), is_cycle.end(), 0);
    cout << count << endl;
    return 0;
}