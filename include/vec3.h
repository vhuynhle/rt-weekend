#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <format>
#include <ostream>

template <std::floating_point T>
class vec3 {
public:
    T e[3];

    vec3()
        : e {}
    {
    }

    vec3(T e0, T e1, T e2)
        : e { e0, e1, e2 }
    {
    }

    T x() const { return e[0]; }

    T y() const { return e[1]; }

    T z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    T operator[](std::size_t i) const { return e[i]; }

    T& operator[](std::size_t i) { return e[i]; }

    vec3& operator+=(const vec3<T>& v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(T t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(T t)
    {
        (*this) *= T { 1.0 } / t;
    }

    T length() const
    {
        return std::sqrt(length_squared());
    }

    T length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
};

using vec3d = vec3<double>;
using vec3f = vec3<float>;
using point3d = vec3d;
using point3f = vec3f;

template <std::floating_point T>
std::ostream& operator<<(std::ostream& ofs, const vec3<T>& v)
{
    ofs << std::format("{} {} {}", v.e[0], v.e[1], v.e[2]);
    return ofs;
}

template <std::floating_point T>
vec3<T> operator+(const vec3<T>& u, const vec3<T>& v)
{
    return vec3<T>(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

template <std::floating_point T>
vec3<T> operator-(const vec3<T>& u, const vec3<T>& v)
{
    return vec3<T>(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

template <std::floating_point T>
vec3<T> operator*(const vec3<T>& u, const vec3<T>& v)
{
    return vec3<T>(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

template <std::floating_point T>
vec3<T> operator*(T t, const vec3<T>& v)
{
    return vec3<T>(t * v.e[0], t * v.e[1], t * v.e[2]);
}

template <std::floating_point T>
vec3<T> operator*(const vec3<T>& v, T t)
{
    return t * v;
}

template <std::floating_point T>
vec3<T> operator/(vec3<T> v, T t)
{
    return (1 / t) * v;
}

template <std::floating_point T>
T dot(const vec3<T>& u, const vec3<T>& v)
{
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

template <std::floating_point T>
vec3<T> cross(const vec3<T>& u, const vec3<T>& v)
{
    return vec3<T>(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template <std::floating_point T>
vec3<T> unit_vector(vec3<T> v)
{
    return v / v.length();
}
