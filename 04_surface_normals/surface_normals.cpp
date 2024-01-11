#include "color.h"
#include "ray.hpp"
#include "vec3.h"

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <optional>

template <std::floating_point T> const point3d kSphereCenter { T(0), T(0), T(-1) };

template <std::floating_point T> constexpr T kSphereRadius { T(0.5) };

template <std::floating_point T>
std::optional<T> hit_sphere(const point3<T>& center, T radius, const ray<T>& r)
{
    const point3<T> oc { r.origin() - center };
    const T a { r.direction().length_squared() };
    const T half_b { dot(oc, r.direction()) };
    const T c { oc.length_squared() - radius * radius };
    const T discriminant { half_b * half_b - a * c };

    if (discriminant < 0) {
        return std::nullopt;
    }

    return (-half_b - std::sqrt(discriminant)) / (a);
}

template <std::floating_point T> color<T> ray_color(const ray<T>& r)
{
    const std::optional<T> may_hit_sphere { hit_sphere(kSphereCenter<T>, kSphereRadius<T>, r) };

    if (may_hit_sphere) {
        const point3<T> hit_point { r.at(*may_hit_sphere) };
        const vec3<T> normal { hit_point - kSphereCenter<T> };
        return color<T> { T(0.5) * normal.x() + T(0.5), T(0.5) * normal.y() + T(0.5),
            T(0.5) * normal.z() + T(0.5) };
    }
    const vec3<T> unit_direction { unit_vector(r.direction()) };
    const T a { T(0.5) * (unit_direction.y() + T(1)) };

    return (T(1) - a) * color<T> { T(1), T(1), T(1) } + a * color<T> { T(0.5), T(0.7), T(1) };
}

int main()
{
    // image
    constexpr double aspect_ratio { 16.0 / 9.0 };
    constexpr std::int32_t image_width { 1024 };
    constexpr std::int32_t image_height { static_cast<std::int32_t>(image_width / aspect_ratio) };

    // camera
    constexpr double focal_length { 1 };
    constexpr double viewport_height { 2.0 };
    constexpr double viewport_width { viewport_height * static_cast<double>(image_width)
        / image_height };
    const point3d camera_center { 0, 0, 0 };

    const vec3d viewport_u { viewport_width, 0, 0 };
    const vec3d viewport_v { 0, -viewport_height, 0 };
    const vec3d pixel_delta_u { viewport_u / static_cast<double>(image_width) };
    const vec3d pixel_delta_v { viewport_v / static_cast<double>(image_height) };

    // The viewport is
    //  + focal_length away from the camera in the -z direction
    //  + to the left of the camera
    //  + to the top of the camera
    const point3d viewport_upper_left { camera_center - vec3d(0, 0, focal_length) - viewport_u / 2.0
        - viewport_v / 2.0 };

    // The center of the pixel (0, 0) is half a pixel away from the corner
    const point3d pixel00_loc { viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v) };

    // Render
    std::cout << std::format("P3\n{} {}\n255\n", image_width, image_height);
    for (std::size_t row { 0 }; row < image_height; ++row) {
        for (std::size_t col { 0 }; col < image_width; ++col) {
            const point3d pixel_center { pixel00_loc + (static_cast<double>(col) * pixel_delta_u)
                + (static_cast<double>(row) * pixel_delta_v) };
            const vec3d ray_direction { pixel_center - camera_center };
            const rayd r { camera_center, ray_direction };
            const colord pixel_color { ray_color(r) };
            write_color(std::cout, pixel_color);
        }
    }

    return 0;
}
