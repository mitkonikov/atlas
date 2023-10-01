struct LCA {
    int n, l;
    vector<vector<int>> adj;

    int timer;
    vector<int> tin, tout;
    vector<vector<int>> up;
    vector<int> depth;

    LCA(vector<vector<int>> adj, int root) {
        this->n = adj.size();
        this->adj = adj;
        tin.resize(n);
        tout.resize(n);
        depth.resize(n);
        timer = 0;
        l = ceil(log2(n));
        up.assign(n, vector<int>(l + 1));
        dfs(root, root);
    }

    ~LCA() {
        tin.clear();
        tout.clear();
        up.clear();
        adj.clear();
    }

    void dfs(int v, int p, int d = 0) {
        tin[v] = ++timer;
        up[v][0] = p;
        depth[v] = d;
        for (int i = 1; i <= l; ++i)
            up[v][i] = up[up[v][i-1]][i-1];

        for (int u : adj[v]) {
            if (u != p)
                dfs(u, v, d + 1);
        }

        tout[v] = ++timer;
    }

    // Is u an ancestor of v
    bool is_ancestor(int u, int v) {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }

    int lca(int u, int v) {
        if (is_ancestor(u, v))
            return u;
        if (is_ancestor(v, u))
            return v;
        for (int i = l; i >= 0; --i) {
            if (!is_ancestor(up[u][i], v))
                u = up[u][i];
        }
        return up[u][0];
    }

    // Jump from u, k steps up
    int jump(int u, int k) {
        for (int i = l; i >= 0; i--) {
            if ((1 << i) & k) {
                u = up[u][i];
            }
        }
        return u;
    }

    bool on_path2(int down, int up, int x) {
        // up <= x <= down
        if (is_ancestor(up, x) && is_ancestor(x, down))
            return true;
        return false;
    }

    // Is x on the path from u to v
    bool on_path(int u, int v, int x) {
        int lc = lca(u, v);
        return (on_path2(u, lc, x) || on_path2(v, lc, x));
    }

    // Distance from u to v given their lca
    int dist(int u, int v, int l) {
        return depth[u] + depth[v] - 2 * depth[l];
    }

    // k-th node on the path from u to v (including)
    int kth(int u, int v, int k) {
        int l = lca(u, v);
        if (k > dist(u, v, l)) return -1;
        if (dist(u, l, l) >= k) return jump(u, k);
        return jump(v, dist(u, v, l) - k);
    }
};