#ifndef SURFACES_H
#define SURFACES_H

#include "real.h"
#include <functional>
#include <cmath>
#include <ostream>

class Point {
public:
    const Real x, y;
    Point(Real x, Real y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << p.x << ' ' << p.y;
        return os;
    }
};

using Surface = std::function<Real(Point)>;

inline Surface plain() {
    return [](Point) { return 0; };
}

inline Surface slope() {
    return [](Point p) { return p.x; };
}

inline Surface steps(Real r = 1) {
    return [=](Point p) {
        return r > 0 ? std::floor(p.x / r) : 0;
    };
}

inline Surface checker(Real r = 1) {
    return [=](Point p) {
        return r > 0 ? std::abs(std::fmod(std::floor(p.x / r), 2))
                       == std::abs(std::fmod(std::floor(p.x / r), 2)) : 0;
    };
}

inline Surface sqr() {
    return [](Point p) { return p.x * p.x; };
}

inline Surface sin_wave() {
    return [](Point p) { return std::sin(p.x); };
}

inline Surface cos_wave() {
    return [](Point p) { return std::cos(p.x); };
}

inline Surface rings(Real r = 1) {
    return [=](Point p) {
        return r > 0 ?
               1 - std::abs(std::fmod(
                       std::ceil((p.x * p.x + p.y * p.y) / r), 2)) : 0;
    };
}

inline Surface ellipse(Real a = 1, Real b = 1) {
    return [=](Point p) {
        return a > 0 && b > 0 ?
               (p.x * p.x / (a * a)) + (p.y * p.y / (b * b)) <= 1 : 0;
    };
}

inline Surface rectangle(Real a = 1, Real b = 1) {
    return [=](Point p) {
        return a > 0 && b > 0 ?
               std::abs(p.x) <= a && std::abs(p.y) <= b : 0;
    };
}

inline Surface stripes(Real r = 1) {
    return [=](Point p) {
        return r > 0 ?
               std::abs(std::fmod(std::ceil(p.x / r), 2)) : 0;
    };
}

inline Surface rotate(const Surface &f, Real deg) {
    return [f, to_rad = -deg * M_PI / 180](Point p) {
        return f(Point(Point(p.x * std::cos(to_rad) - p.y * std::sin(to_rad),
                             p.x * std::sin(to_rad) + p.y * std::cos(to_rad))));
    };
}

inline Surface translate(const Surface &f, Point v) {
    return [=](Point p) { return f(Point(p.x - v.x, p.y - v.y)); };
}

inline Surface scale(const Surface &f, Point s) {
    return [=](Point p) { return f(Point(p.x / s.x, p.y / s.y)); };
}

inline Surface invert(const Surface &f) {
    return [=](Point p) { return f(Point(p.y, p.x)); };
}

inline Surface flip(const Surface &f) {
    return [=](Point p) { return f(Point(-1 * p.x, p.y)); };
}

inline Surface mul(const Surface &f, Real r) {
    return [=](Point p) { return f(p) * r; };
}

inline Surface add(const Surface &f, Real r) {
    return [=](Point p) { return f(p) + r; };
}

template <typename H, typename... F>
inline auto evaluate(const H& h, const F&... f) {
    return [=](Point p){ return h(f(p)...); };
}

inline auto compose() {
    return [](auto a) { return a; };
}

template <typename F>
inline auto compose(const F &f) {
    return f;
}

template <typename F1, typename F2>
inline auto compose (const F1 &f1, const F2 &f2) {
    return [=] (auto... args) {return f2(f1(args...)); };
}

template <typename F1, typename F2, typename... Fs>
inline auto compose(const F1 &f1, const F2 &f2, const Fs&... fs) {
    return compose(compose(f1, f2), fs...);
}

#endif // SURFACES_H