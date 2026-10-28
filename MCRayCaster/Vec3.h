
#pragma once

#include <cmath>

struct Vec3 {
    double x;
    double y;
    double z;
};

inline Vec3 add_vec(const Vec3& a, const Vec3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
inline Vec3 subtraction(const Vec3& a, const Vec3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}
inline Vec3 scale_vec(const Vec3& v, double s) {
    return { v.x * s, v.y * s, v.z * s };
}
inline Vec3 multiply_vec(const Vec3& a, const Vec3& b) {
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}
inline double dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}
inline Vec3 normalize(const Vec3& v) {
    const double length = std::sqrt(dot(v, v));
    return length > 0.0 ? scale_vec(v, 1.0 / length) : Vec3{ 0, 0, 0 };
}
