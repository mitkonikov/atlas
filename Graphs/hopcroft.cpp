// hopcroft_karp.cpp , Eric K. Zhang; Nov. 22, 2017
#include <bits/stdc++.h>
using namespace std;

int c, b, n, p;

vector<vector<int>> adj;
vector<int> match;
vector<int> dist;
bool bfs() {
	queue<int> q;
	fill(dist.begin(), dist.end(), -1);
	for (int i = 0; i < n; i++) {
		if (match[i] == -1) {
			q.push(i);
			dist[i] = 0;
		}
	}
	bool reached = false;
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int v : adj[u]) {
			if (match[v] == -1) reached = true;
			else if (dist[match[v]] == -1) {
				dist[match[v]] = dist[u] + 1;
				q.push(match[v]);
			}
		}
	}
	return reached;
}
bool dfs(int u) {
	if (u == -1) return true;
	for (int v : adj[u]) {
		if (match[v] == -1 || dist[match[v]] == dist[u] + 1) {
			if (dfs(match[v])) {
				match[v] = u, match[u] = v;
				return true;
			}
		}
	}
	return false;
}
int hopcroft_karp() {
	fill(match.begin(), match.end(), -1);
	int matching = 0;
	while (bfs()) {
		for (int i = 0; i < n; i++)
			if (match[i] == -1 && dfs(i))
				matching++;
	}
	return matching;
}
int main() {
	// http://www.spoj.com/problems/MATCHING/
	cin >> c >> b >> p; // C cows, B bulls, P pairs
	n = c + b; // N = C + B
	adj.resize(n);
	dist.resize(n);
	match.resize(n);
	for (int i = 0; i < p; i++) {
		int A, B; cin >> A >> B; --A, --B;
		adj[A].push_back(c + B);
		adj[c + B].push_back(A);
	}
	// the max pairs that can be matched
	cout << hopcroft_karp() << endl;
	return 0;
}