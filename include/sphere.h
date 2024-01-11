#pragma once

#include "hittable.h"
#include "ray.hpp"
#include "vec3.h"
#include <cmath>
#include <concepts>
#include <optional>

template <std::floating_point T>
class sphere : public hittable<T> {
public:
    sphere(point3<T> center, T radius)
        : center_ { center }
        , radius_ { radius } { }

    std::optional<hit_record<T>> hit(const ray<T>& r, T ray_tmin, T ray_tmax) const override {
        const vec3<T> oc { r.origin() - center_ };
        const T a { r.direction().length_squared() };
        const T half_b { dot(oc, r.direction()) };
        const T c { oc.length_squared() - radius_ * radius_ };
        const T discriminant { half_b * half_b - a * c };

        if (discriminant < 0) {
            return std::nullopt;
        }

        const T sqrtd { std::sqrt(discriminant) };

        // Find the nearest root inside the acceptable range
        T root { (-half_b - sqrtd) / a };
        if (root <= ray_tmin || root >= ray_tmax) {
            root = (-half_b + sqrtd) / a;
            if (root <= ray_tmin || root >= ray_tmax) {
                return std::nullopt;
            }
        }

        const point3<T> hit_point { r.at(root) };
        const vec3<T> normal { (hit_point - center_) / radius_ };
        return std::make_optional<hit_record<T>>(hit_point, normal, root);
    }

private:
    point3<T> center_;
    T radius_;
};
