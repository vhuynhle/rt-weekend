#pragma once

#include "hittable.h"
#include "interval.h"
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

    std::optional<hit_record<T>> hit(const ray<T>& r, interval<T> rayt) const override {
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
        if (!rayt.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!rayt.surrounds(root)) {
                return std::nullopt;
            }
        }

        const point3<T> hit_point { r.at(root) };
        const vec3<T> outward_normal { (hit_point - center_) / radius_ };
        const auto [front_face, normal] = get_face_normal(r, outward_normal);
        return std::make_optional<hit_record<T>>(hit_point, normal, root, front_face);
    }

private:
    point3<T> center_;
    T radius_;
};
