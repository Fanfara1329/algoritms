#include <bits/stdc++.h>

using namespace std;
#define int long long
#define ld long double
int P1 = 1e6 + 29;
ld EPS = 1e-6;


struct Line;

struct Point {
    ld x, y;

    Point() {
        x = 0;
        y = 0;
    }

    Point(ld x, ld y) {
        this->x = x;
        this->y = y;
    }

    friend istream &operator>>(istream &input, Point &P);

    friend ostream &operator<<(ostream &output, const Point &P);

    ld dist(Point B) {
        return sqrt((x - B.x) * (x - B.x) + (y - B.y) * (y - B.y));
    }

    void SetDist(ld R) {
        ld len = dist({0ll, 0ll});
        x = x / len * R;
        y = y / len * R;
    }

    Point median(Point P, ld a, ld b) {
        return {(P.x * a + x * b) / (a + b), (P.y * a + y * b) / (a + b)};
    }

    Point orthocenter(Point P, Point Q) {
        ld m = -1 / (Q.x - P.x) / (Q.y - P.y);
    }

    ld vec(Point p) {
        return (x * p.y - y * p.x);
    }

};

void print(vector<vector<Point>> v) {
    for (auto i : v) {
        for (auto j : i)
            cout << j << "   -   ";
        cout << "\n";
    }
    cout << "\n";
}


Point mx;

struct Line {
    ld a, b, c;

    Line() {
        a = 0;
        b = 0;
        c = 0;
    }

    Line(Point A, Point B) {
        a = B.y - A.y;
        b = A.x - B.x;
        c = B.x * A.y - A.x * B.y;
    }

    Line(Point P, Line l) {
        Point v = {l.a, l.b};
        Point Q = {P.x + v.x, P.y + v.y};
        Line ans = {P, Q};
        a = ans.a;
        b = ans.b;
        c = ans.c;
    }

    friend istream &operator>>(istream &input, Line &P);

    friend ostream &operator<<(ostream &output, const Line &P);

    Point GetNormal() {
        return {a, b};
    }

    bool IsOnLine(Point A) {
        return abs(a * A.x + b * A.y + c) <= EPS;
    }

    ld dist(Point P) {
        return abs(a * P.x + b * P.y + c) / sqrt(a * a + b * b);
    }

    Point GetPoint() {
        if (abs(a) < EPS)
            return {0, -c / b};
        else
            return {-c / a, 0};
    }

    bool IsOnSame(Point A, Point B) {

        if (a * A.x + b * A.y + c <= EPS and a * B.x + b * B.y + c <= EPS or a * A.x + b * A.y + c >= -EPS and
            a * B.x + b * B.y + c >= -EPS)
            return true;
        else
            return false;
    }

    int Inter(Line l, Point &A) {
        ld d0 = a * l.b - b * l.a, d1 = l.c * b - c * l.b, d2 = l.a * c - a * l.c;
        if (abs(d0) < EPS and abs(d1) < EPS and abs(d2) < EPS)
            return 2;
        else if (abs(d0) < EPS)
            return 0;
        else {
            A.x = d1 / d0;
            A.y = d2 / d0;
            return 1;
        }
    }
};

struct Beam {
    Point S, F;
    Line l, lp;

    Beam() {

    }

    Beam(Point S, Point F) {
        this->S = S;
        this->F = F;
        l = {F, S};
        lp = {S, l};
    }

    bool IsOnBeam(Point P) {
        return lp.IsOnSame(P, F) && l.IsOnLine(P);
    }

    bool IsOnBeamA(Point P) {
        return lp.IsOnSame(P, F);
    }

    ld dist(Point P) {
        if (IsOnBeamA(P))
            return l.dist(P);
        else
            return S.dist(P);
    }

};

struct Segment {
    Point S, F;
    Line l, ls, lf;

    Segment() {}

    Segment(Point S, Point F) {
        this->S = S;
        this->F = F;
        l = {S, F};
        ls = {S, l};
        lf = {F, l};
    }

    bool InOnSegS(Point P) {
        return ls.IsOnSame(P, F);
    }

    bool InOnSegF(Point P) {
        return lf.IsOnSame(P, S);
    }

    ld dist(Point P) {
        if (!InOnSegF(P))
            return F.dist(P);
        else if (!InOnSegS(P))
            return S.dist(P);
        else
            return l.dist(P);
    }
};

struct Circle {
    Point O;
    ld R;

    Circle() {}

    Circle(Point O, ld R) {
        this->O = O;
        this->R = R;
    }

    friend istream &operator>>(istream &input, Circle &O);

    friend ostream &operator<<(ostream &output, const Circle &O);

    vector<Point> Interline(Line l) {
        if (abs(l.dist(O) - R) < EPS) {
            Line ll = {O, l};
            Point A;
            ll.Inter(l, A);
            return {A};
        } else if (l.dist(O) > R)
            return {};
        else {
            Line ll = {O, l};
            Point A;
            ll.Inter(l, A);
            ld d = sqrt(R * R - l.dist(O) * l.dist(O));
            Point n = {ll.a, ll.b};
            n.SetDist(d);
            return {{A.x - n.x, A.y - n.y},
                    {A.x + n.x, A.y + n.y}};
        }
    }

    vector<Point> tangent(Point P) {
        if (abs(P.dist(O) - R) < EPS)
            return {P};
        else if (P.dist(O) < R)
            return {};
        else {
            Point a = {O.x - P.x, O.y - P.y};
            ld q = O.dist(P);
            ld v = sqrt(q * q - R * R);
            ld AC = v * v / q;
            a.SetDist(AC);
            Point B = {P.x - a.x, P.y - a.y}, C = {P.x + a.x, P.y + a.y};
            if (C.dist(O) < B.dist(O))
                swap(B, C);
            ld r = v / q * R;
            Line l = {O, P};
            Point n = {l.a, l.b};
            n.SetDist(r);
            return {{B.x - n.x, B.y - n.y},
                    {B.x + n.x, B.y + n.y}};
        }
    }
};

vector<Point> InterCircle(Circle a, Circle b, int &t) {
    if (abs(a.O.x - b.O.x) < EPS && abs(a.O.y - b.O.y) < EPS && abs(a.R - b.R) < EPS) {
        t = 1;
        return {};
    } else if (abs(a.R + b.R - a.O.dist(b.O)) < EPS || abs(a.O.dist(b.O) + b.R - a.R) < EPS) {
        Point n = {b.O.x - a.O.x, b.O.y - a.O.y};
        n.SetDist(a.R);
        Point x = {a.O.x - n.x, a.O.y - n.y}, y = {a.O.x + n.x, a.O.y + n.y};
        if (x.dist(b.O) < y.dist(b.O))
            return {x};
        else
            return {y};
    } else if (a.R + b.R < a.O.dist(b.O) || a.O.dist(b.O) + b.R < a.R)
        return {};
    else {
        ld dist = a.O.dist(b.O);
        Line l = {a.O, b.O};
        if (dist > a.R) {
            ld x = (dist * dist - a.R * a.R + b.R * b.R) / (2 * dist);
            ld q = sqrt(b.R * b.R - x * x);
            Point X = {a.O.x - b.O.x, a.O.y - b.O.y};
            X.SetDist(x);
            Point n1 = {b.O.x + X.x, b.O.y + X.y}, n2 = {b.O.x + X.x, b.O.y + X.y};
            if (n2.dist(a.O) < n1.dist(a.O))
                swap(n1, n2);
            Point n = {l.a, l.b};
            n.SetDist(q);
            return {{n1.x + n.x, n1.y +n.y}, {n1.x - n.x, n1.y -n.y}};
        }
        else {
            ld x = (a.R * a.R - b.R * b.R - dist * dist) / (2 * dist);
            ld q = sqrt(b.R * b.R - x * x);
            Point n = {b.O.x - a.O.x, b.O.y - a.O.y};
            n.SetDist(dist + x);
            Point E = {a.O.x - n.x, a.O.y - n.y}, E1 = {a.O.x + n.x, a.O.y + n.y};
            if (E1.dist(b.O) < E.dist(b.O))
                swap(E, E1);
            Point Q = {l.a, l.b};
            Q.SetDist(q);
            return {{E.x - Q.x, E.y - Q.y}, {E.x + Q.x, E.y + Q.y}};
        }
    }
}


signed main() {
    cout.precision(16);
    Circle a, b;
    cin >> a >> b;
    if (b.R > a.R)
        swap(a, b);
    int t = 0;
    vector<Point> vec = InterCircle(a, b, t);
    if (t) {
        cout << 3;
        return 0;
    }
    cout << vec.size() << "\n";
    for (auto i : vec)
        cout << i << "\n";
    return 0;
}

istream &operator>>(istream &input, Point &P) {
    input >> P.x >> P.y;
    return input;
}

ostream &operator<<(ostream &output, const Point &P) {
    output << P.x << ' ' << P.y;
    return output;
}

istream &operator>>(istream &input, Line &P) {
    input >> P.a >> P.b >> P.c;
    return input;
}

ostream &operator<<(ostream &output, const Line &P) {
    output << P.a << ' ' << P.b << ' ' << P.c;
    return output;
}

istream &operator>>(istream &input, Circle &O) {
    input >> O.O >> O.R;
    return input;
}

ostream &operator<<(ostream &output, const Circle &O) {
    output << O.O << ' ' << O.R;
    return output;
}

/*
10
1 1
0 6
15 3
4 3
3 4
9 6
8 2
6 9
13 9
3 7
*/