#include <bits/stdc++.h>
#define ll long long
#define endl '\n'

using namespace std;

#define SMALL_INF (ll)1e17

template<typename T>
class MaxFlow {
public:
    int n;
    vector<vector<T>> adj;
    vector<vector<T>> capacity; // stores the residual flow
    vector<vector<T>> flows; // stores the flow
    vector<vector<bool>> direction; // stores the edges direction
    T INF = numeric_limits<T>::max();

    struct Flow {
        T node;
        T flow;
    };

    MaxFlow(int n) {
        this->n = n;
        this->adj.resize(n, vector<T>());
        this->capacity.resize(n, vector<T>(n, 0));
        this->direction.resize(n, vector<bool>(n, false));
        this->flows.resize(n, vector<T>(n, 0));
    }

    void addEdge(int u, int v, T c) {
        direction[u][v] = true;
        capacity[u][v] += c;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    T bfs(int source, int sink, vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[source] = -2; // NULL value
        queue<Flow> q;
        q.push({ source, INF });

        while(!q.empty()) {
            T currentNode = q.front().node;
            T flow = q.front().flow;
            q.pop();

            for (int nextNode: adj[currentNode]) {
                if (parent[nextNode] == -1 && capacity[currentNode][nextNode] > 0) {
                    parent[nextNode] = currentNode;
                    T new_flow = min(flow, capacity[currentNode][nextNode]);
                    if (nextNode == sink) return new_flow;
                    q.push({ nextNode, new_flow });
                    assert(new_flow != INF);
                }
            }
        }

        return 0;
    }

    T getMaxFlow(int source, int sink) {
        vector<int> parent(n);
        T flow = 0;
        T new_flow;

        while (new_flow = bfs(source, sink, parent)) {
            flow += new_flow;
            T current = sink;
            while (current != source) {
                int prev = parent[current];
                flows[prev][current] += new_flow;
                flows[current][prev] -= new_flow;
                capacity[prev][current] -= new_flow;
                capacity[current][prev] += new_flow;
                current = prev; // go back
            }
        }

        return flow;
    };

    map<pair<int, int>, T> getEdges() {
        map<pair<int, int>, T> result; // edge (u, v) mapped to flow
        for (int i = 0; i < (int)adj.size(); i++) {
            for (int j = 0; j < (int)adj[i].size(); j++) {
                int nextNode = adj[i][j];
                if (flows[i][nextNode] > 0 && direction[i][nextNode]) {
                    result[{ i, nextNode }] = flows[i][nextNode];
                }
            }
        }

        return result;
    }

    set<int> getCut(int source, int sink) {
        set<int> nodes;
        queue<int> q;
        q.push(source);
        vector<bool> visited(n, false);
        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            visited[node] = true;
            for (auto v: adj[node]) {
                if (!visited[v] && capacity[node][v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (visited[i]) {
                for (auto v: adj[i]) {
                    if (flows[i][v] > 0 && !visited[v]) {
                        nodes.insert(v);
                    }
                }
            }
        }

        nodes.erase(source);
        nodes.erase(sink);
        return nodes;
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    int source = 1, sink = (n - 1) * 2;

    MaxFlow<ll> mf(n * 2 + 2);

    for (int i = 0; i < m; i++) {
        int s, e; cin >> s >> e;
        s--; e--;

        mf.addEdge(2 * s + 1, 2 * e, SMALL_INF);
        mf.addEdge(2 * e + 1, 2 * s, SMALL_INF);
    }

    for (int i = 0; i < n; i++) {
        int c;
        cin >> c;
        mf.addEdge(2 * i, 2 * i + 1, c);
        mf.addEdge(2 * i + 1, 2 * i, c);
    }

    ll flow = mf.getMaxFlow(source, sink);
    cout << flow << endl;

    set<int> vertices = mf.getCut(source, sink);
    vertices.erase(0);
    cout << vertices.size() << endl;
    for (auto v: vertices) {
        cout << (v-1) / 2 + 1 << " ";
    } cout << endl;

    cout << flush;
    
    return 0;
}