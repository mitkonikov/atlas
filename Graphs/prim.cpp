// Prim's Algorithm is used to find the Minimum Spanning Tree (MST) of a graph, 
// minimizing the total edge weight while connecting all vertices without cycles.

using namespace std;

struct PrimAlgorithm {
    int n; // number of vertices
    vector<vector<pair<int, int>>> adj;

    PrimAlgorithm(int n) : n(n), adj(n) {}

    void addEdge(int u, int v, int weight) {
        adj[u].push_back(make_pair(v, weight));
        adj[v].push_back(make_pair(u, weight));
    }

    int minimumSpanningTree(int start = 0) {
        vector<bool> inMST(n, false);
        vector<int> minEdge(n, 999999);
        minEdge[start] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        pq.push(make_pair(0, start));

        int mstWeight = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            int weight = pq.top().first;
            pq.pop();

            if (inMST[u]) continue;

            inMST[u] = true;
            mstWeight += weight;

            for (auto [v, w]: adj[u]) {

                if (!inMST[v] && w < minEdge[v]) {
                    minEdge[v] = w;
                    pq.push(make_pair(w, v));
                }
            }
        }

        return mstWeight;
    }
};

int main() {
    int n = 5;
    PrimAlgorithm solver(n);

    solver.addEdge(0, 1, 2);
    solver.addEdge(0, 3, 6);
    solver.addEdge(1, 2, 3);
    solver.addEdge(1, 3, 8);
    solver.addEdge(1, 4, 5);
    solver.addEdge(2, 4, 7);
    solver.addEdge(3, 4, 9);

    int mstWeight = solver.minimumSpanningTree();
    cout << "Minimum Spanning Tree Weight: " << mstWeight << endl;

    return 0;
}
