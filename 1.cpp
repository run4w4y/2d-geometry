#include <bits/stdc++.h>

using namespace std;

const double EPS = 1e-9;

bool doubleEqual (long double a, long double b) {
    return fabs(a - b) < EPS;
}

bool doubleLessOrEqual (long double a, long double b) {
    return a < b || doubleEqual(a, b);
}

bool doubleLess(long double a, long double b) {
    return a < b && !doubleEqual(a, b);
}

bool doubleGreaterOrEqual (long double a, long double b) {
    return a > b || doubleEqual(a, b);
}

bool doubleGreater (long double a, long double b) {
    return a > b && !doubleEqual(a, b);
}

long double mySqrt (long double a) {
    if (doubleLess(a, 0)) {
        throw "sqrt(-1)";
    }
    if (a < 0) return 0;
    return sqrt(a);
}

struct pt {
private:
public:
    long double x, y;
    pt() : x(0), y(0) {}
    pt(long double x, long double y) : x(x), y(y) {}
    void scan() {
        cin >> x >> y;
    }
    pt operator+ (const pt & p) const {
        return pt(x + p.x, y + p.y);
    }
    pt operator- (const pt & p) const {
        return pt(x - p.x, y - p.y);
    }
    pt operator- () const {
        return pt(-x, -y);
    }
    pt operator* (long double k) const {
        return pt(x*k, y*k);
    }
    pt operator/ (long double k) const {
        return pt(x/k, y/k);
    }
    long double operator% (const pt & p) const { // scalar product
        return x * p.x + y * p.y;
    }
    long double operator* (const pt & p) const { // vector product
        return x * p.y - y * p.x;
    }
    long double length() const {
        return mySqrt(*this % *this);
    }
    long double distTo(const pt & p) const { // distance from this point to another
        return (*this - p).length();
    }
    long double distTo(const pt & A, const pt & B) { // distance from this point to line
        long double d = A.distTo(B);
        if (doubleEqual(d, 0)) {
            throw "A = B";
        }
        long double s = (*this - A) * (*this - B);
        return fabs(s)/d;
    }
    bool is_on_line (const pt & A, const pt & B) const {
        return doubleEqual((A - *this) * (B - *this), 0);
    }
    bool is_in_seg (const pt & A, const pt & B) const {
        return is_on_line(A, B) && doubleLessOrEqual((A - *this) % (B - *this), 0);
    }
};

struct line {
private:
public:
    long double a, b, c;
    line(pt &d, pt &e) {
        a = d.y - e.y;
        b = e.x - d.x;
        c = -a * d.x - b * d.y;
    }
};

long double det (long double a, long double b, long double c, long double d) {
    return a * d - b * c;
}

bool intersect (line m, line n, pt &res) {
    long double zn = det (m.a, m.b, n.a, n.b);
    if (abs (zn) < EPS)
        return false;
    res.x = - det (m.c, m.b, n.c, n.b) / zn;
    res.y = - det (m.a, m.c, n.a, n.c) / zn;
    return true;
}

bool parallel (line m, line n) {
    return abs (det (m.a, m.b, n.a, n.b)) < EPS;
}

bool equivalent (line m, line n) {
    return abs(det(m.a, m.b, n.a, n.b)) < EPS
           && abs(det(m.a, m.c, n.a, n.c)) < EPS
           && abs(det(m.b, m.c, n.b, n.c)) < EPS;
}
pt closest_point (line l, pt p) {
    long double k = (l.a * p.x + l.b * p.y + l.c) / (l.a * l.a + l.b * l.b);
    return pt(p.x - l.a * k, p.y - l.b * k);
}

int point_in_line (line l, pt p) {
    long double s = l.a * p.x + l.b * p.y + l.c;
    return s < - EPS ? - 1 : s > EPS ? 1 : 0;
}

bool point_in_ray (pt p, pt p1, pt p2) {
    line l = line(p1, p2);
    if (point_in_line(l, p) != 0) return false;

    if (abs (l.b) <= EPS)
        if (p2.y >= p1.y)
            return p.y >= p1.y;
        else
            return p.y <= p1.y;

    if (p2.x >= p1.x)
        return p.x >= p1.x;
    else
        return p.x <= p1.x;
}

long double dist_from_pt_to_ray (pt p, pt p1, pt p2) {
    line l = line(p1, p2);
    pt t = closest_point(l, p);
    if (point_in_ray (t, p1, p2))
        return p.distTo(t);
    return p.distTo(p1);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout << setprecision(6);
    pt p1, p2, p3, p4, res = pt();
    p1.scan();
    p2.scan();
    p3.scan();
    p4.scan();
    line line1 = line(p1, p2), line2 = line(p3, p4);
    if (equivalent(line1, line2)) {
        if (point_in_ray(p1, p3, p4) or point_in_ray(p3, p1, p2)) cout << 0; else cout << p1.distTo(p3);
        return 0;
    }
    if (parallel(line1, line2)) {
        cout << p1.distTo(p3);
        return 0;
    }
    if (intersect(line1, line2, res)){
        if (point_in_ray(res, p1, p2) && point_in_ray(res, p3, p4)) {
            cout << 0;
            return 0;
        }
    }
    cout << min(dist_from_pt_to_ray(p3, p1, p2), dist_from_pt_to_ray(p1, p3, p4));
}