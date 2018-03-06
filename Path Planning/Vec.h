#ifndef VEC_H_
#define VEC_H_

#include <math.h>
#include <stdlib.h>
#include <iostream>

/* Vec2 is pulled from IRIS-2017. */
template<typename T>
struct Vec2
{
    Vec2() {}
    Vec2(T xi, T yi=0)
    {
        x = xi;
        y = yi;
    }
    float length() const
    {
        return sqrt(x*x+y*y);
    }
    void normalize()//sets Vec unit length to 1
    {
        float dist = length();
        x /= dist;
        y /= dist;
    }
    T x;
    T y;
};
/* define a useful float pt type */
typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

/* float 3-d class for vectorized operations. */
class Vec3f{
    public:
        Vec3f() {}
        Vec3f(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }
        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }

        inline const Vec3f& operator+() const { return *this; }
        inline Vec3f operator-() const { return Vec3f(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) const { return e[i]; }
        inline float& operator[](int i) { return e[i]; };

        inline Vec3f& operator+=(const Vec3f &v2);
        inline Vec3f& operator-=(const Vec3f &v2);
        inline Vec3f& operator*=(const Vec3f &v2);
        inline Vec3f& operator/=(const Vec3f &v2);
        inline Vec3f& operator*=(const float t);
        inline Vec3f& operator/=(const float t);

        inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
        inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
        inline void make_unit_vector();


        float e[3];
};



inline std::istream& operator>>(std::istream &is, Vec3f &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3f &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void Vec3f::make_unit_vector() {
    float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vec3f operator+(const Vec3f &v1, const Vec3f &v2) {
    return Vec3f(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3f operator-(const Vec3f &v1, const Vec3f &v2) {
    return Vec3f(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3f operator*(const Vec3f &v1, const Vec3f &v2) {
    return Vec3f(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3f operator/(const Vec3f &v1, const Vec3f &v2) {
    return Vec3f(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3f operator*(float t, const Vec3f &v) {
    return Vec3f(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3f operator/(Vec3f v, float t) {
    return Vec3f(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline Vec3f operator*(const Vec3f &v, float t) {
    return Vec3f(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline float dot(const Vec3f &v1, const Vec3f &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}

inline Vec3f cross(const Vec3f &v1, const Vec3f &v2) {
    return Vec3f( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
                (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}


inline Vec3f& Vec3f::operator+=(const Vec3f &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

inline Vec3f& Vec3f::operator*=(const Vec3f &v){
    e[0]  *= v.e[0];
    e[1]  *= v.e[1];
    e[2]  *= v.e[2];
    return *this;
}

inline Vec3f& Vec3f::operator/=(const Vec3f &v){
    e[0]  /= v.e[0];
    e[1]  /= v.e[1];
    e[2]  /= v.e[2];
    return *this;
}

inline Vec3f& Vec3f::operator-=(const Vec3f& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

inline Vec3f& Vec3f::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

inline Vec3f& Vec3f::operator/=(const float t) {
    float k = 1.0/t;

    e[0]  *= k;
    e[1]  *= k;
    e[2]  *= k;
    return *this;
}

inline Vec3f unit_vector(Vec3f v) {
    return v / v.length();
}

#endif
