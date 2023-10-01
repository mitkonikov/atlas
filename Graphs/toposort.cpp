#include <bits/stdc++.h>
using namespace std;

struct TopologicalSort {
    vector<vector<int>> adj;
    vector<bool> visited;
    vector<int> sorted;
    int n;

    TopologicalSort(int n) {
        this->n = n;
        adj.resize(n);
        visited.resize(n, false);
        sorted.reserve(n);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int v) {
        visited[v] = true;
        for (auto u: adj[v]) {
            if (!visited[u]) {
                dfs(u);
            }
        }

        sorted.push_back(v);
    }

    void sort() {
        for (int i = 0; i < n; i++) {
            if (!visited[i]) dfs(i);
        }

        reverse(sorted.begin(), sorted.end());
    }
};
 
int main()
{
    int n, m;
    cin >> n >> m;
    TopologicalSort g(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        g.addEdge(u, v);
    }
 
    g.sort();
    
    for (int i = 0; i < n; i++) {
        cout << g.sorted[i] + 1 << " ";
    } cout << endl;
 
    return 0;
}