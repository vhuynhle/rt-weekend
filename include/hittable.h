#pragma once

#include "ray.hpp"
#include "vec3.h"

#include <concepts>
#include <optional>
#include <utility>

template <std::floating_point T>
struct hit_record {
    point3<T> p;     // The origin of the normal vector
    vec3<T> normal;  // The unit normal vector, pointing against the the hitting ray
    T t;             // Scale
    bool front_face; // Record if the ray comes from the outside (front face)
};

/**
 * Check if a ray is coming from the front face or from the back face of a surface.
 *
 * @param r
 *        The ray to consider
 * @outward_normal
 *        The normal of the surface, pointing from the inside to the outside
 * @return A pair:
 *        - The first element indicates if the ray is coming from the outside (front face) or from
 * the inside (back face)
 *        - The second element is the normal vector adjusted so that it always pointing against the
 * ray.
 */
template <std::floating_point T>
static std::pair<bool, vec3<T>> get_face_normal(const ray<T>& r, const vec3<T>& outward_normal) {
    bool front_face { dot(r.direction(), outward_normal) < 0 };
    const vec3<T> normal { front_face ? outward_normal : -outward_normal };
    return { front_face, normal };
}

template <std::floating_point T>
class hittable {
public:
    virtual ~hittable() = default;
    virtual std::optional<hit_record<T>> hit(const ray<T>& r, T ray_tmin, T ray_tmax) const = 0;
};
