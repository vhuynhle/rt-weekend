#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <concepts>
#include <cstddef>
#include <iostream>

template <std::floating_point T>
class camera {
public:
    camera(const T aspect_ratio_, std::size_t image_width_, point3<T> center_, T focal_length_,
           T viewport_height_)
        : aspect_ratio { aspect_ratio_}
        , image_width { image_width_}
        , image_height { static_cast<std::size_t>(image_width / aspect_ratio) }
        , center { center_ }
        , focal_length { focal_length_ }
        , viewport_height { viewport_height_ }
        , viewport_width { viewport_height * static_cast<T>(image_width)
                           / static_cast<T>(image_height) }
        , viewport_u { viewport_width, 0, 0 }
        , viewport_v { 0, -viewport_height, 0 }
        , pixel_delta_u{viewport_u / static_cast<T>(image_width)}
        , pixel_delta_v{viewport_v / static_cast<T>(image_height)}
        , viewport_upper_left{center - vec3<T>{0, 0, focal_length} - viewport_u / T(2) - viewport_v / T(2)}
        , pixel00_loc{viewport_upper_left + T(0.5) * (pixel_delta_u + pixel_delta_v)} { }

    void render(const hittable<T>& world) const {
        std::cout << std::format("P3\n{} {}\n255\n", image_width, image_height);
        for (std::size_t row { 0 }; row < image_height; ++row) {
            for (std::size_t col { 0 }; col < image_width; ++col) {
                const point3d pixel_center { pixel00_loc
                                             + (static_cast<double>(col) * pixel_delta_u)
                                             + (static_cast<double>(row) * pixel_delta_v) };
                const vec3d ray_direction { pixel_center - center };
                const rayd r { center, ray_direction };
                const colord pixel_color { ray_color(world, r) };
                write_color(std::cout, pixel_color);
            }
        }
    }

private:
    const T aspect_ratio;
    const std::size_t image_width;
    const std::size_t image_height;
    const point3<T> center;
    const T focal_length;
    const T viewport_height;
    const T viewport_width;
    const vec3<T> viewport_u;
    const vec3<T> viewport_v;
    const vec3<T> pixel_delta_u;
    const vec3<T> pixel_delta_v;
    const point3<T> viewport_upper_left;
    const point3<T> pixel00_loc;

    color<T> ray_color(const hittable<T>& obj, const ray<T>& r) const {
        const std::optional<hit_record<T>> may_hit_obj { obj.hit(r, interval<T>::nonnegative) };

        if (may_hit_obj) {
            const vec3<T> normal { may_hit_obj->normal };
            return color<T> { T(0.5) * normal.x() + T(0.5), T(0.5) * normal.y() + T(0.5),
                              T(0.5) * normal.z() + T(0.5) };
        }
        const vec3<T> unit_direction { unit_vector(r.direction()) };
        const T a { T(0.5) * (unit_direction.y() + T(1)) };

        return (T(1) - a) * color<T> { T(1), T(1), T(1) } + a * color<T> { T(0.5), T(0.7), T(1) };
    }
};
