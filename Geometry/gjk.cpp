#include <bits/stdc++.h>
using namespace std;

#define ld long double
#define vpt vector<Point<T>>

template<class T>
struct Point {
    typedef Point P;
    T x, y;
    explicit Point(T x=0, T y=0) : x(x), y(y) {}
    P operator+(P p) const { return P(x+p.x, y+p.y); }
    P operator-(P p) const { return P(x-p.x, y-p.y); }
    P operator*(T d) const { return P(x*d, y*d); }
    P operator/(T d) const { return P(x/d, y/d); }
    P negate() const { return P(-x, -y); }
    T dot(P p) const { return x*p.x + y*p.y; }
    T dist2() const { return x*x + y*y; }
    
    P perp() const { return P(-y, x); } // rotates +90 degrees
    P perpCW() const { return P(y, -x); } // rotates -90 degrees <<<

    static P tripleProduct(P a, P b, P c) {
        return b * (a.dot(c)) - a * (b.dot(c));
    }

    static P average(const vector<P> &points) {
        P avg(0, 0);
        for (int i = 0; i < points.size(); i++) {
            avg = avg + points[i];
        }

        return avg / points.size();
    }
};

template<typename T>
int furthestPoint(const vpt &points, Point<T> d) {
    T maxProduct = d.dot(points[0]); // it could be negative!
    int index = 0;
    for (int i = 1; i < points.size(); i++) {
        T product = d.dot(points[i]);
        if (product > maxProduct) {
            maxProduct = product;
            index = i;
        }
    }
    return index;
}

template<typename T>
Point<T> support(const vpt &v1, const vpt &v2, Point<T> d) {
    int i = furthestPoint<T>(v1, d);
    int j = furthestPoint<T>(v2, d.negate());
    return v1[i] - v2[j];
}

int iter_count = 0;

template<typename T>
bool GJK(const vpt &v1, const vpt &v2) {
    int index = 0;

    Point<T> a, b, c, d, ao, perp_prod, simplex[3];
    
    Point<T> pos1 = Point<T>::average(v1);
    Point<T> pos2 = Point<T>::average(v2);
    d = pos1 - pos2;

    if (d.x == 0 && d.y == 0) {
        d.x = 1;
    }

    a = simplex[0] = support(v1, v2, d);
    if (a.dot(d) <= 0) return false;
    d = a.negate();

    while (true) {
        iter_count++;

        a = simplex[++index] = support(v1, v2, d);
        if (a.dot(d) <= 0) return false;
        ao = a.negate();

        if (index < 2) {
            b = simplex[0];
            d = Point<T>::tripleProduct(b-a, ao, b-a);
            if (d.dist2() == 0) d = (b-a).perpCW();
            continue;
        }

        b = simplex[1];
        c = simplex[0];

        perp_prod = Point<T>::tripleProduct(b-a, c-a, c-a);
        if (perp_prod.dot(ao) >= 0) {
            d = perp_prod;
        } else {
            perp_prod = Point<T>::tripleProduct(c-a, b-a, b-a);
            if (perp_prod.dot(ao) < 0) return true;
            simplex[0] = simplex[1];
            d = perp_prod;
        }

        simplex[1] = simplex[2];
        --index;
    }

    return false;
}

int main() {
    // Point<ld> vertices1[] = {
    //     Point<ld>(4.0f, 11.0f),
    //     Point<ld>(5.0f, 5.0f),
    //     Point<ld>(9.0f, 9.0f)
    // };

    // Point<ld> vertices2[] = {
    //     Point<ld>(4.0f, 11.0f),
    //     Point<ld>(5.0f, 5.0f),
    //     Point<ld>(9.0f, 9.0f)
    // };

    // // NO COLLISION
    // Point<ld> vertices1[] = {
    //     Point<ld>(4.0f, 8.0f),
    //     Point<ld>(5.0f, 5.0f),
    //     Point<ld>(1.0f, 2.0f)
    // };
    
    // Point<ld> vertices2[] = {
    //     Point<ld>(2.0f, 2.0f),
    //     Point<ld>(4.0f, 4.0f),
    //     Point<ld>(8.0f, 6.0f)
    // };

    int n, m;
    cin >> n >> m;

    vector<Point<ld>> v1(n), v2(m);
    for (int i = 0; i < n; i++) {
        cin >> v1[i].x >> v1[i].y;
    }

    for (int i = 0; i < n; i++) {
        cin >> v2[i].x >> v2[i].y;
    }

    int collisionDetected = GJK<ld>(v1, v2);
    cout << (collisionDetected ? "YES" : "NO") << endl;
    return 0;
}