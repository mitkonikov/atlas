//
// 3D Coordinate-Wise Domination
//
// Description:
//   Point (x,y,z) dominates (x',y',z') if
//     x < x', y < y', and z < z'
//   holds. Kung-Luccio-Preparata proposed an algorithm to compute
//   the all set of dominating points in O(n log n) time.
//
// Complexity:
//   O(n log n). By using this method recursively,
//   we can solve d-dimensional domination in O(n log^{d-2} n).
//
// Reference:
//   Hsiang-Tsung Kung, Fabrizio Luccio, Franco P. Preparata (1975):
//   "On finding the maxima of a set of vectors." Journal of the ACM,
//   vol.22, no.4, pp.469-476.
//
// Implementation Author:
//   Mitko Nikov
//
// Tested:
//   https://mendo.mk/Task.do?id=912 & stress-tested
//

#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y, z;
};

int domination(vector<Point> v) {
    int n = v.size();
    int ans = 0;
    sort(v.begin(), v.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return (a.x > b.x);
        if (a.y != b.y) return (a.y < b.y); // this is very important
        return (a.z < b.z);
    });

    auto cond = [&](pair<int, int> a, pair<int, int> b) {
        return (a.first > b.first && a.second > b.second);
    };

    // Maintains a data structure, where x is mapped to 
    // the max(y) seen with this x while holding that
    // if x2 > x1 then y2 < y1 for each (x[i], y[i])
    map<int, int> frontier; // { x -> max(y) }
    auto update = [&](Point p) {
        auto it = frontier.find(p.y);
        if (it != frontier.end()) {
            it->second = max(it->second, p.z);
        } else {
            frontier[p.y] = p.z;
            it = frontier.find(p.y);
        }
        
        while (it != frontier.begin() 
            && prev(it)->second <= it->second) {
            frontier.erase(prev(it));
        }

        if (next(it) != frontier.end() && cond(*next(it), { p.y, p.z })) {
            if (it->second <= p.z) frontier.erase(it);
            return true;
        } else {
            return false;
        }
    };

    for (int i = 0; i < n; i++) {
        if (update(v[i])) {
            ans++;
        }
    }
    return ans;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<Point> ps(n);
    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d", &ps[i].x, &ps[i].y, &ps[i].z);
    }
    printf("%d\n", domination(ps));
}
