#pragma once

#include "ray.hpp"
#include "vec3.h"

#include <concepts>
#include <optional>

template <std::floating_point T>
struct hit_record {
    point3<T> p;    // The origin of the normal vector
    vec3<T> normal; // The unit normal vector
    T t;            // Scale
};

template <std::floating_point T>
class hittable {
public:
    virtual ~hittable() = default;
    virtual std::optional<hit_record<T>> hit(const ray<T>& r, T ray_tmin, T ray_tmax) const = 0;
};
