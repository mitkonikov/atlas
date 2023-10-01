#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> adj;
vector<vector<int>> tree_adj;

// All vectors should be resized to size N
vector<int> visited; // Topological Sort
vector<int> sorted;  // Output

vector<bool> visited_comp;
vector<vector<int>> comps;

int n, m;
void dfs(int v) {
    visited[v] = true;
    for (auto u: adj[v]) {
        if (!visited[u]) {
            dfs(u);
        }
    }
    sorted.push_back(v);
}

void dfs_comp(int u, int p, int c) {
    comps[c].push_back(u);
    visited_comp[u] = true;
    for (auto v: tree_adj[u]) {
        if (v == p || visited_comp[v]) continue;
        dfs_comp(v, u, c);
    }
}

void sort() {
    // Classify the nodes by the component they are in.
    // Time Complexity is still O(N)
    int comp_id = 0;
    for (int i = 0; i < n; i++) {
        if (visited_comp[i]) continue;
        dfs_comp(i, -1, comp_id);
        comp_id++;
    }

    for (int i = 0; i < comp_id; i++) {
        for (auto u: comps[i]) {
            if (visited[u]) continue;
            dfs(u); // This is the same function as above.
        }
    }

    // You may want to reverse each component separately
    reverse(sorted.begin(), sorted.end());
}

int main() {
    cin >> n >> m;
    adj.resize(n);
    tree_adj.resize(n);
    visited.resize(n);
    visited_comp.resize(n);
    comps.resize(n);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u].push_back(v);
        tree_adj[u].push_back(v);
        tree_adj[v].push_back(u);
    }

    sort();
    for (int i = 0; i < n; i++) {
        cout << sorted[i] + 1 << " ";
    } cout << endl;

    return 0;
}