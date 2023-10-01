#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
using Point = pair<ll, ll>;
const ll INF = LLONG_MAX / 4;
const double inv_phi = (sqrt(5) - 1) / 2;
void chmax(ll& a, ll b){ if(a < b) a = b; }
 
ll ccw(Point a, Point b, Point c){
    ll cross = (b.first - a.first) * (c.second - b.second) - (b.second - a.second) * (c.first - b.first);
    if (cross > 0) return 1;
    else if (cross == 0) return 0;
    else return -1;
}

struct ConvexHull{
    vector<Point> lower, upper;
    ConvexHull(Point p): lower{p}, upper{p}{}
    ConvexHull(const ConvexHull& a, const ConvexHull& b){
        vector<Point> v;
        merge(a.lower.begin(), a.lower.end(),
              b.lower.begin(), b.lower.end(), back_inserter(v));
        for (Point p : v){
            while(lower.size() >= 2 && ccw(lower.rbegin()[1], lower.back(), p) <= 0){
                lower.pop_back();
            }
            lower.push_back(p);
        }
        v.clear();
        merge(a.upper.begin(), a.upper.end(), 
              b.upper.begin(), b.upper.end(), back_inserter(v));
        for (Point p : v) {
            while(upper.size() >= 2 && ccw(upper.rbegin()[1], upper.back(), p) >= 0){
                upper.pop_back();
            }
            upper.push_back(p);
        }
    }

    ll get(ll A, ll B) const {
        auto& s = B < 0 ? lower : upper;
        // here's the eval function
        auto f = [&](ll i){ return A * s[i].first + B * s[i].second; };
        // golden-section search
        ll l = 0, r = s.size() - 1, r2 = round(r * inv_phi), f_r2 = f(r2);
        while(abs(l - r) >= 6){
            ll l2 = r + llround((l - r) * inv_phi), f_l2 = f(l2);
            if(f_l2 < f_r2) tie(l, r) = tuple{r, l2};
            else tie(r, r2, f_r2) = tuple{r2, l2, f_l2};
        }
        ll ans = -INF;
        if(l > r) swap(l, r);
        for(ll i = l; i <= r; i++) chmax(ans, f(i));
        return ans;
    }
};

int main(){
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    ll Q;
    cin >> Q;
    vector<ConvexHull> S;
    for(ll i = 1; i <= Q; i++){
        ll X, Y, A, B;
        cin >> X >> Y >> A >> B;
        S.emplace_back(pair{X, Y});
        for(ll j = 1; (i & j) == 0; j <<= 1){
            S.rbegin()[1] = ConvexHull(S.rbegin()[1], S.back());
            S.pop_back();
        }
        ll ans = -INF;
        for(auto& s : S) chmax(ans, s.get(A, B));
        cout << ans << '\n';
    }
}