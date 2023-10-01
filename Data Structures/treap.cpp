/**
 * Author: someone on Codeforces
 * Date: 2017-03-14
 * Source: folklore
 * Description: A short self-balancing tree. It acts as a
 *  sequential container with log-time splits/joins, and
 *  is easy to augment with additional data.
 * Time: $O(\log N)$
 * Status: stress-tested
 */
#include <bits/stdc++.h>
#include <random>
#include <chrono>
#define ll long long
using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
auto unif = uniform_int_distribution<int>(0, 1000000);

struct Node {
	Node *l = 0, *r = 0;
	// change to ll if necessary
	int val, y, c = 1; // c - subtree size
	int lazy = 0; // to propagate
	ll sum = 0;
	Node(int val) {
		this->val = val;
		this->sum = val;
		this->y = unif(rng);
	}

	void prop() {
		if (this->lazy == 0) return;
		this->val += lazy;

		if (l) {
			this->l->lazy += this->lazy;
		}

		if (r) {
			this->r->lazy += this->lazy;
		}

		this->lazy = 0;
		recalc();
	}

	void recalc() {
		this->c = 1;
		this->sum = this->val + this->lazy;

		if (l) {
			this->c += l->c;
			this->sum += l->lazy * l->c + l->sum;
		}

		if (r) {
			this->c += r->c;
			this->sum += r->lazy * r->c + r->sum;
		}
	}
};

struct Treap {
	int cnt(Node* n) { return n ? n->c : 0; }
	
	template<class F> void each(Node* n, F f) {
		if (n) { 
			n->prop();
			each(n->l, f);
			f(n->val);
			each(n->r, f);
		}
	}

	pair<Node*, Node*> split(Node* n, int k) {
		if (!n) return {};
		n->prop();
		if (cnt(n->l) >= k) { // "n->val >= k" for lower_bound(k)
			auto pa = split(n->l, k);
			n->l = pa.second;
			n->recalc();
			return {pa.first, n};
		} else {
			auto pa = split(n->r, k - cnt(n->l) - 1); // and just "k"
			n->r = pa.first;
			n->recalc();
			return {n, pa.second};
		}
	}

	Node* merge(Node* l, Node* r) {
		if (!l) return r;
		if (!r) return l;
		l->prop(); 
		r->prop();
		if (l->y > r->y) {
			l->r = merge(l->r, r);
			l->recalc();
			return l;
		} else {
			r->l = merge(l, r->l);
			r->recalc();
			return r;
		}
	}

	Node* insert(Node* t, Node* n, int pos) {
		auto pa = split(t, pos);
		return merge(merge(pa.first, n), pa.second);
	}

	// Move the range [l, r) to index k
	void move(Node*& t, int l, int r, int k) {
		Node *a, *b, *c;
		tie(a,b) = split(t, l); tie(b,c) = split(b, r - l);
		if (k <= l) t = merge(insert(a, b, k), c);
		else t = merge(a, insert(c, b, k - r));
	}

	void rangeAdd(Node*& root, int l, int r, int add) {
		assert(l <= r);
		auto [L, mid] = split(root, l);
		auto [mid2, R] = split(mid, r-l+1);
		mid2->lazy += add;
		root = merge(L, merge(mid2, R));
	}

	int rangeSum(Node*& root, int l, int r) {
		assert(l <= r);
		auto [L, mid] = split(root, l);
		auto [mid2, R] = split(mid, r-l+1);
		int res = mid2->sum;
		root = merge(L, merge(mid2, R));
		return res;
	}

	Node* createTreap(const vector<int> &v) {
		assert(!v.empty());
		Node* root = new Node(v[0]);
		for (int i = 1; i < v.size(); i++) {
			Node* right = new Node(v[i]);
			root = merge(root, right);
		}
		return root;
	}
};

int main() {
	int n, q; cin >> n >> q;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	Treap t;
	Node* root = t.createTreap(a);
	for (int i = 0; i < q; i++) {
		int type;
		cin >> type;
		if (type == 0) {
			int x, y, z;
			cin >> x >> y >> z;
			x--; y--;
			t.rangeAdd(root, x, y, z);
		} else {
			int x, y;
			cin >> x >> y;
			x--; y--;
			cout << t.rangeSum(root, x, y) << endl;
		}
	}

	return 0;
}