
#include "color.h"
#include "ray.hpp"
#include "vec3.h"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>

template <std::floating_point T>
const auto kSphereCenter = point3<T> { T { 0 }, T { 0 }, T { -1 } };

template <std::floating_point T> constexpr auto kSphereRadius = static_cast<T>(0.5);

template <std::floating_point T> bool hit_sphere(const point3<T>& center, T radius, const ray<T>& r)
{
    const auto oc = r.origin() - center;
    const auto a = dot(r.direction(), r.direction());
    const auto b = 2.0 * dot(oc, r.direction());
    const auto c = dot(oc, oc) - radius * radius;
    const auto delta = b * b - 4 * a * c;
    return delta > 0;
}

template <std::floating_point T> color<T> ray_color(const ray<T>& r)
{
    if (hit_sphere(kSphereCenter<T>, kSphereRadius<T>, r)) {
        return color<T> { 1, 0, 0 };
    }
    const vec3<T> unit_direction = unit_vector(r.direction());
    const T a = T { 0.5 } * (unit_direction.y() + T { 1.0 });
    return (T { 1 } - a) * color<T> { T { 1.0 }, T { 1.0 }, T { 1.0 } }
    + a * color<T> { T { 0.5 }, T { 0.7 }, T { 1.0 } };
}

int main()
{
    // image
    constexpr double aspect_ratio { 16.0 / 9.0 };
    constexpr std::int32_t image_width { 1024 };
    constexpr std::int32_t image_height { static_cast<std::int32_t>(image_width / aspect_ratio) };

    // camera
    constexpr double focal_length { 1.0 };
    constexpr double viewport_height { 2.0 };
    constexpr double viewport_width { viewport_height * static_cast<double>(image_width)
        / image_height };
    const point3d camera_center { 0.0, 0.0, 0.0 };

    const vec3d viewport_u { viewport_width, 0, 0 };
    const vec3d viewport_v { 0, -viewport_height, 0 };
    const vec3d pixel_delta_u = viewport_u / static_cast<double>(image_width);
    const vec3d pixel_delta_v = viewport_v / static_cast<double>(image_height);

    const point3d viewport_upper_left = camera_center
        - vec3d(0, 0,
            focal_length)  // The viewport is focal_length away from the camera, in the -z direction
        - viewport_u / 2.0 // the viewport is to the left of the camera
        - viewport_v / 2.0; // the view port is to the top of the camera

    const point3d pixel00_loc = viewport_upper_left
        + 0.5 * (pixel_delta_u + pixel_delta_v); // half a pixel away from the corner

    // Render
    std::cout << std::format("P3\n{} {}\n255\n", image_width, image_height);
    for (std::size_t row { 0 }; row < image_height; ++row) {
        for (std::size_t col { 0 }; col < image_width; ++col) {
            const point3d pixel_center = pixel00_loc + (static_cast<double>(col) * pixel_delta_u)
                + (static_cast<double>(row) * pixel_delta_v);
            const vec3d ray_direction = pixel_center - camera_center;
            const rayd r(camera_center, ray_direction);
            const colord pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    return 0;
}
