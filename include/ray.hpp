#pragma once

#include <concepts>

#include "vec3.h"

template <std::floating_point T>
class ray {
public:
    ray() { }

    ray(const point3<T>& origin,
        const vec3<T>& direction)
        : orig { origin }
        , dir { direction }
    {
    }

    point3<T> origin() const { return orig; }

    vec3<T> direction() const { return dir; }

    point3<T> at(T t) const {
        return orig + t * dir;
    }
private:
    point3<T> orig;
    vec3<T> dir;
};
