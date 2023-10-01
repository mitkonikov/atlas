vector<vector<int>> adj;

vector<int> color;

bool isBipartite(int n) {
    color.resize(n, -1);
    color[0] = 1;
 
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
 
        for (auto v: adj[u]) {
            if (v == u) return false;
            if (color[v] == -1) {
                color[v] = 1 - color[u];
                q.push(v);
            } else if (color[v] == color[u]) return false;
        }
    }
 
    return true;
}