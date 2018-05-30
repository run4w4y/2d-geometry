#include <bits/stdc++.h>

using namespace std;

const double EPS = 1e-9; // epsilon

bool doubleEqual (long double a, long double b) { // these functions are using epsilon
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

struct pt { // structure for points
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
    bool ccw(pt a, pt b);
};

struct line { // structure for lines
private:
public:
    long double a, b, c; // ax + by + c = 0
    line(pt &d, pt &e) { // making line from two points
        a = d.y - e.y;
        b = e.x - d.x;
        c = -a * d.x - b * d.y;
    }
};

long double det(long double a, long double b, long double c, long double d) {
    return a * d - b * c;
}

bool intersect(line m, line n, pt &res) { // check if lines have intersection and where it is
    long double zn = det (m.a, m.b, n.a, n.b);
    if (abs (zn) < EPS)
        return false;
    res.x = - det (m.c, m.b, n.c, n.b) / zn;
    res.y = - det (m.a, m.c, n.a, n.c) / zn;
    return true;
}

bool parallel(line m, line n) { // check if lines are parallel
    return abs (det (m.a, m.b, n.a, n.b)) < EPS;
}

bool equivalent(line m, line n) { // check if lines are equivalent
    return abs(det(m.a, m.b, n.a, n.b)) < EPS
           && abs(det(m.a, m.c, n.a, n.c)) < EPS
           && abs(det(m.b, m.c, n.b, n.c)) < EPS;
}
pt closest_point(line l, pt p) { // find the closest point on the line
    long double k = (l.a * p.x + l.b * p.y + l.c) / (l.a * l.a + l.b * l.b);
    return pt(p.x - l.a * k, p.y - l.b * k);
}

int point_in_line(line l, pt p) { // check if point is in line
    long double s = l.a * p.x + l.b * p.y + l.c;
    return s < - EPS ? - 1 : s > EPS ? 1 : 0;
}

bool point_in_ray(pt p, pt p1, pt p2) { // check if point is in ray
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

long double dist_from_pt_to_ray(pt p, pt p1, pt p2) { // find distance between point and ray
    line l = line(p1, p2);
    pt t = closest_point(l, p);
    if (point_in_ray (t, p1, p2))
        return p.distTo(t);
    return p.distTo(p1);
}

long double area_triangle(pt a, pt b, pt c) { // oriented area of a triangle
    return 0.5 * (a.x * b.y + b.x * c.y + c.x * a.y - a.y * b.x - b.y * c.x - c.y * a.x);
}

bool pt :: ccw(pt a, pt b) { // is the point to the right of the segment in the counterclockwise direction?
    return area_triangle(*this, a, b) > EPS;
}

long double dist_between_rays (pt p1, pt p2, pt p3, pt p4) { // distance between two rays
    pt res = pt();
    // p1 - start of the first ray
    // p2 - any point in the first ray
    // p3 - start of the second ray
    // p4 - any point from the second ray
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
    return min(dist_from_pt_to_ray(p3, p1, p2), dist_from_pt_to_ray(p1, p3, p4));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cout << fixed << setprecision(6);
}
