#include <bits/stdc++.h>
#define ll long long
using namespace std;

struct Node {
	int val;
	Node *l, *r;

	Node(ll x) : val(x), l(nullptr), r(nullptr) {}
	Node(Node *L, Node *R) : val(0), l(L), r(R) {}
};

int n, init[100001]; // The initial array and its size
Node* roots[100001]; // The persistent array's roots

Node* build(int l = 0, int r = n - 1) {
	if (l == r) return new Node(init[l]);
	int mid = (l + r) / 2;
	return new Node(build(l, mid), build(mid + 1, r));
}

Node* update(Node* node, int val, int pos, int l = 0, int r = n - 1) {
	if (l == r) return new Node(val);
	int mid = (l + r) / 2;
	if (pos > mid) return new Node(node->l, update(node->r, val, pos, mid + 1, r));
	else return new Node(update(node->l, val, pos, l, mid), node->r);
}

int query(Node* node, int pos, int l = 0, int r = n - 1) {
	if (l == r) return node->val;
	int mid = (l + r) / 2;
	if (pos > mid) return query(node->r, pos, mid + 1, r);
	return query(node->l, pos, l, mid);
}

// Gets the array item at a given index and time
int get_item(int time, int index) {
	return query(roots[time], index);
}

// Updates the array item at a given index and time
void update_item(int prev_time, int curr_time, int index, int value) {
	roots[curr_time] = update(roots[prev_time], index, value);
}

// Initializes the persistent array, given an input array
void init_arr(int nn, vector<int> initial_array) {
	n = nn;
	for (int i = 0; i < n; i++)
		init[i] = initial_array[i];
	roots[0] = build();
}

int main() {
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    init_arr(n, v);

    for (int i = 0; i < 5; i++) {
        update_item(i, i+1, i, i);
    }

    for (int time = 0; time < 5; time++) {
        for (int i = 0; i < n; i++) {
            cout << get_item(time, i) << " ";
        }
        cout << endl;
    }
    return 0;
}