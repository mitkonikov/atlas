const double PI = acos(-1.0);

// implementation note: use EPS only in this function
// usage note: check sign(x) < 0, sign(x) > 0, or sign(x) == 0
const double EPS = 1e-8;
int sign(double x) {
  if (x < -EPS) return -1;
  if (x > +EPS) return +1;
  return 0;
}

using real = long double;
struct point {
  real x, y;
  point &operator+=(point p) { x += p.x; y += p.y; return *this; }
  point &operator-=(point p) { x -= p.x; y -= p.y; return *this; }
  point &operator*=(real a)     { x *= a;   y *= a;   return *this; }
  point &operator/=(real a)     { return *this *= (1.0/a); }
  point operator-() const    { return {-x, -y}; }
  bool operator<(point p) const {
    int s = sign(x - p.x);
    return s ? s < 0 : sign(y - p.y) < 0;
  }
};
bool operator==(point p, point q) { return !(p < q) && !(q < p); }
bool operator!=(point p, point q) { return p < q || q < p; }
bool operator<=(point p, point q) { return !(q < p); }
point operator+(point p, point q) { return p += q; }
point operator-(point p, point q) { return p -= q; }
point operator*(real a, point p) { return p *= a; }
point operator*(point p, real a) { return p *= a; }
point operator/(point p, real a) { return p /= a; }
real dot(point p, point q) { return p.x*q.x+p.y*q.y; }
real cross(point p, point q) { return p.x*q.y-p.y*q.x; } // left turn > 0
real norm2(point p) { return dot(p,p); }
point orth(point p) { return {-p.y, p.x}; }
real norm(point p) { return sqrt(dot(p,p)); }
real arg(point p) { return atan2(p.y, p.x); }
real arg(point p, point q){ return atan2(cross(p,q), dot(p,q)); }

istream &operator>>(istream &is, point &p) { is>>p.x>>p.y;return is; }
ostream &operator<<(ostream &os, const point &p) { os<<"("<<p.x<<","<<p.y<<")"; return os; }
typedef vector<point> polygon;


// exact comparison by polar angle
// usage: sort(all(ps), polar_angle(origin, direction));
struct polar_angle {
  const point o;
  const int s; // +1 for ccw, -1 for cw
  polar_angle(point p = {0,0}, int s = +1) : o(p), s(s) { }
  int quad(point p) const {
    for (int i = 1; i <= 4; ++i, swap(p.x = -p.x, p.y))
      if (p.x > 0 && p.y >= 0) return i;
    return 0;
  }
  bool operator()(point p, point q) const {
    p = p - o; q = q - o;
    if (quad(p) != quad(q)) return s*quad(p) < s*quad(q);
    if (cross(p, q)) return s*cross(p, q) > 0;
    return norm2(p) < norm2(q); // closer first
  }
};

struct line { point p, q; };
bool operator==(line l, line m) {
  return !sign(cross(l.p-l.q,m.p-m.q)) && !sign(cross(l.p-l.q,m.p-l.p));
}

struct segment { point p, q; };
bool operator==(segment l, line m) {
  return (l.p==m.p && l.q==m.q) || (l.p==m.q && l.q==m.p); // do not consider the direction
}
struct circle { point p; real r; };
bool operator==(circle c, circle d) { return c.p == d.p && !sign(c.r - d.r); }


//-----------------------------------------------------------------------------
// triangulate simple polygon in O(n) time.
//
// [non-verified]; future work for polygonal overlay
//-----------------------------------------------------------------------------
real triangulate(vector<point> ps) {
  int n = ps.size();
  vector<int> next(n);
  for (int i = 0; i < n-1; ++i) next[i] = i+1;
  auto is_ear = [&](int i, int j, int k) {
    if (sign(cross(ps[j]-ps[i], ps[k]-ps[i])) <= 0) return false;
    for (int l = next[k]; l != i; l = next[l])
      if (sign(cross(ps[i]-ps[l], ps[j]-ps[l])) >= 0
       && sign(cross(ps[j]-ps[l], ps[k]-ps[l])) >= 0
       && sign(cross(ps[k]-ps[l], ps[i]-ps[l])) >= 0) return false;
    return true;
  };
  real area = 0;
  for (int i = 0; next[next[i]] != i; ) {
    if (is_ear(i, next[i], next[next[i]])) {
      area  += abs(cross(ps[next[i]]-ps[i], ps[next[next[i]]] - ps[i])) / 2;
      next[i] = next[next[i]];
    } else i = next[i];
  }
  return area;
}

//-----------------------------------------------------------------------------
// area of intersection of two circles
// [verified]
//-----------------------------------------------------------------------------
real intersection_area(circle c, circle d) {
  if (c.r < d.r) swap(c, d);
  auto A = [&](real r, real h) {
    return r*r*acos(h/r)-h*sqrt(r*r-h*h);
  };
  auto l = norm(c.p - d.p), a = (l*l + c.r*c.r - d.r*d.r)/(2*l);
  if (sign(l - c.r - d.r) >= 0) return 0; // far away
  if (sign(l - c.r + d.r) <= 0) return PI*d.r*d.r;
  if (sign(l - c.r) >= 0) return A(c.r, a) + A(d.r, l-a);
  else return A(c.r, a) + PI*d.r*d.r - A(d.r, a-l);
}

//-----------------------------------------------------------------------------
// circle-polygon intersection area
// [verified]
//-----------------------------------------------------------------------------
real intersection_area(vector<point> ps, circle c) {
  auto tri = [&](point p, point q){
    point d = q - p;
    auto a = dot(d,p)/dot(d,d), b = (dot(p,p)-c.r*c.r)/dot(d,d);
    auto det = a*a - b;
    if (det <= 0) return arg(p,q) * c.r*c.r / 2;
    auto s = max(0.l, -a-sqrt(det)), t = min(1.l, -a+sqrt(det));
    if (t < 0 || 1 <= s) return c.r*c.r*arg(p,q)/2;
    point u = p + s*d, v = p + t*d;
    return arg(p,u)*c.r*c.r/2 + cross(u,v)/2 + arg(v,q)*c.r*c.r/2;
  };
  auto sum = 0.0;
  for (int i = 0; i < ps.size(); ++i)
    sum += tri(ps[i] - c.p, ps[(i+1)%ps.size()] - c.p);
  return sum;
}
real intersection_area(circle c, vector<point> ps) {
  return intersection_area(ps, c);
}

//-----------------------------------------------------------------------------
// find the closest pair of points by sweepline
// [verified]
//-----------------------------------------------------------------------------
pair<point,point> closest_pair(vector<point> ps) {
  sort(all(ps), [](point p, point q) { return p.y < q.y; });
  auto u = ps[0], v = ps[1];
  auto best = dot(u-v, u-v);
  auto update = [&](point p, point q) {
    auto dist = dot(p-q, p-q);
    if (best > dist) { best = dist; u = p; v = q; }
  };
  set<point> S; S.insert(u); S.insert(v);
  for (int l = 0, r = 2; r < ps.size(); ++r) {
    if (S.count(ps[r])) return {ps[r], ps[r]};
    if ((ps[l].y-ps[r].y)*(ps[l].y-ps[r].y) > best) S.erase(ps[l++]);
    auto i = S.insert(ps[r]).fst;
    for (auto j = i; ; ++j) {
      if (j == S.end() || (i->x-j->x)*(i->x-j->x) > best) break;
      if (i != j) update(*i, *j);
    }
    for (auto j = i; ; --j) {
      if (i != j) update(*i, *j);
      if (j == S.begin() || (i->x-j->x)*(i->x-j->x) > best) break;
    }
  }
  return {u, v};
}


//-----------------------------------------------------------------------------
// find a circle of radius r that contains many points as possible
//
// quad-tree search (this is faster than the next sweepline solution)
// [verified: AOJ 1132]
//-----------------------------------------------------------------------------
int maximum_circle_cover(vector<point> ps, double r) {
  const double dx[] = {1,-1,-1,1}, dy[] = {1,1,-1,-1};
  point best_p;
  int best = 0;
  function<void(point,double,vector<point>)>
    rec = [&](point p, double w, vector<point> ps) {
    w /= 2;
    point qs[4];
    vector<point> pss[4];
    for (int i = 0; i < 4; ++i) {
      qs[i] = p + w * point({dx[i], dy[i]});
      int lo = 0;
      for (point q: ps) {
        auto d = dist(qs[i], q);
        if (sign(d - r) <= 0) ++lo;
        if (sign(d - w*sqrt(2) - r) <= 0) pss[i].push_back(q);
      }
      if (lo > best) { best = lo; best_p = qs[i]; }
    }
    for (int i = 0, j; i < 4; ++i) {
      for (int j = i+1; j < 4; ++j)
        if (pss[i].size() < pss[j].size())
          swap(pss[i], pss[j]), swap(qs[i], qs[j]);
      if (pss[i].size() <= best) break;
      rec(qs[i], w, pss[i]);
    }
  };
  real w = 0;
  for (point p: ps) w = max(w, max(abs(p.x), abs(p.y)));
  rec({0,0}, w, ps);
  return best; //best_p;
}

//-----------------------------------------------------------------------------
// area of union of rectangles
// Bentley's sweepline with segment tree.
//
// [accepted, LightOJ 1120 Rectangle Union]
//-----------------------------------------------------------------------------
struct rectangle { point p, q; }; // lower-left and upper-right
real rectangle_union(vector<rectangle> rs) {
  vector<real> ys; // plane sweep with coordinate compression
  struct event {
    real x, ymin, ymax;
    int add;
    bool operator<(event e) const { return x < e.x; }
  };
  vector<event> es;
  for (auto r: rs) {
    ys.push_back(r.p.y);
    ys.push_back(r.q.y);
    es.push_back({r.p.x, r.p.y, r.q.y, +1});
    es.push_back({r.q.x, r.p.y, r.q.y, -1});
  }
  sort(all(es));
  sort(all(ys));
  ys.erase(unique(all(ys)), ys.end());

  vector<real> len(4 * ys.size()); // segment tree on sweepline
  vector<int> sum(4 * ys.size());
  function<void(real, real, int, int,int,int)> update
    = [&](real ymin, real ymax, int add, int i, int j, int k) {
    ymin = max(ymin, ys[i]); ymax = min(ymax, ys[j]);
    if (ymin >= ymax) return;
    if (ys[i] == ymin && ys[j] == ymax) sum[k] += add;
    else {
      update(ymin, ymax, add, i, (i+j)/2, 2*k+1);
      update(ymin, ymax, add, (i+j)/2, j, 2*k+2);
    }
    if (sum[k]) len[k] = ys[j] - ys[i];
    else        len[k] = len[2*k+1] + len[2*k+2];
  };
  real area = 0;
  for (int i = 0; i+1 < es.size(); ++i) {
    update(es[i].ymin, es[i].ymax, es[i].add, 0, ys.size()-1, 0);
    area += len[0] * (es[i+1].x - es[i].x);
  }
  return area;
}