#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "utils.hpp"
#include "vec3.hpp"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <iostream>

template <std::floating_point T>
class camera {
public:
    camera(const T aspect_ratio_, std::size_t image_width_, point3<T> center_, T focal_length_,
           T viewport_height_, std::int32_t samples_per_pixel_)
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
        , pixel00_loc{viewport_upper_left + T(0.5) * (pixel_delta_u + pixel_delta_v)}
        ,samples_per_pixel{samples_per_pixel_}
    { }

    void render(const hittable<T>& world) const {
        std::cout << std::format("P3\n{} {}\n255\n", image_width, image_height);
        for (std::size_t row { 0 }; row < image_height; ++row) {
            for (std::size_t col { 0 }; col < image_width; ++col) {
                color<T> pixel_color {};
                for (std::int32_t i { 0 }; i < samples_per_pixel; ++i) {
                    const ray<T> r { get_ray(row, col) };
                    pixel_color += ray_color(world, r);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
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
    const std::int32_t samples_per_pixel;

    color<T> ray_color(const hittable<T>& obj, const ray<T>& r) const {
        const std::optional<hit_record<T>> may_hit_obj { obj.hit(r, interval<T>::nonnegative) };

        if (may_hit_obj) {
            const vec3<T> normal { may_hit_obj->normal };

            // Gray diffuse material
            const vec3<T> direction { random_on_hemisphere(normal) };
            return T(0.7) * ray_color(obj, ray<T> { may_hit_obj->p, direction });

            // return color<T> { T(0.5) * normal.x() + T(0.5), T(0.5) * normal.y() + T(0.5),
            //                   T(0.5) * normal.z() + T(0.5) };
        }
        const vec3<T> unit_direction { unit_vector(r.direction()) };
        const T a { T(0.5) * (unit_direction.y() + T(1)) };

        return (T(1) - a) * color<T> { T(1), T(1), T(1) } + a * color<T> { T(0.5), T(0.7), T(1) };
    }

    ray<T> get_ray(std::size_t row, std::size_t col) const {
        const point3<T> pixel_center = pixel00_loc + (static_cast<T>(col) * pixel_delta_u)
                                     + (static_cast<T>(row) * pixel_delta_v);
        const T px = get_random<T>() - T(0.5);
        const T py = get_random<T>() - T(0.5);
        const vec3<T> dx = px * pixel_delta_u;
        const vec3<T> dy = py * pixel_delta_v;
        const point3<T> pixel_sample = pixel_center + dx + dy;

        const point3<T> ray_origin = center;
        const point3<T> ray_direction = pixel_sample - ray_origin;

        return ray<T> { ray_origin, ray_direction };
    }
};
