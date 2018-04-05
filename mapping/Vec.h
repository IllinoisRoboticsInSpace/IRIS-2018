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
typedef Vec2<float> cvec2f;
typedef Vec2<int> cvec2i;

/* float 3-d class for vectorized operations. */
class cvec3f{
    public:
        cvec3f() {}
        cvec3f(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }
        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }

        inline const cvec3f& operator+() const { return *this; }
        inline cvec3f operator-() const { return cvec3f(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) const { return e[i]; }
        inline float& operator[](int i) { return e[i]; };

        inline cvec3f& operator+=(const cvec3f &v2);
        inline cvec3f& operator-=(const cvec3f &v2);
        inline cvec3f& operator*=(const cvec3f &v2);
        inline cvec3f& operator/=(const cvec3f &v2);
        inline cvec3f& operator*=(const float t);
        inline cvec3f& operator/=(const float t);

        inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
        inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
        inline void make_unit_vector();


        float e[3];
};



inline std::istream& operator>>(std::istream &is, cvec3f &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const cvec3f &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void cvec3f::make_unit_vector() {
    float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k; e[1] *= k; e[2] *= k;
}

inline cvec3f operator+(const cvec3f &v1, const cvec3f &v2) {
    return cvec3f(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline cvec3f operator-(const cvec3f &v1, const cvec3f &v2) {
    return cvec3f(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline cvec3f operator*(const cvec3f &v1, const cvec3f &v2) {
    return cvec3f(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline cvec3f operator/(const cvec3f &v1, const cvec3f &v2) {
    return cvec3f(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline cvec3f operator*(float t, const cvec3f &v) {
    return cvec3f(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline cvec3f operator/(cvec3f v, float t) {
    return cvec3f(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

inline cvec3f operator*(const cvec3f &v, float t) {
    return cvec3f(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline float dot(const cvec3f &v1, const cvec3f &v2) {
    return v1.e[0] *v2.e[0] + v1.e[1] *v2.e[1]  + v1.e[2] *v2.e[2];
}

inline cvec3f cross(const cvec3f &v1, const cvec3f &v2) {
    return cvec3f( (v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1]),
                (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
                (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0]));
}


inline cvec3f& cvec3f::operator+=(const cvec3f &v){
    e[0]  += v.e[0];
    e[1]  += v.e[1];
    e[2]  += v.e[2];
    return *this;
}

inline cvec3f& cvec3f::operator*=(const cvec3f &v){
    e[0]  *= v.e[0];
    e[1]  *= v.e[1];
    e[2]  *= v.e[2];
    return *this;
}

inline cvec3f& cvec3f::operator/=(const cvec3f &v){
    e[0]  /= v.e[0];
    e[1]  /= v.e[1];
    e[2]  /= v.e[2];
    return *this;
}

inline cvec3f& cvec3f::operator-=(const cvec3f& v) {
    e[0]  -= v.e[0];
    e[1]  -= v.e[1];
    e[2]  -= v.e[2];
    return *this;
}

inline cvec3f& cvec3f::operator*=(const float t) {
    e[0]  *= t;
    e[1]  *= t;
    e[2]  *= t;
    return *this;
}

inline cvec3f& cvec3f::operator/=(const float t) {
    float k = 1.0/t;

    e[0]  *= k;
    e[1]  *= k;
    e[2]  *= k;
    return *this;
}

inline cvec3f unit_vector(cvec3f v) {
    return v / v.length();
}

#endif
