#pragma once

#include "vec3.hpp"

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <ostream>
#include <type_traits>

template <std::floating_point T>
using color = vec3<T>;
using colorf = color<float>;
using colord = color<double>;

template <typename T, typename U>
    requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U>
constexpr U scale(T value, T old_max, U new_max) {
    return std::clamp<T>(static_cast<double>(value) * new_max / old_max, 0, new_max);
}

template <std::floating_point T>
void write_color(std::ostream& ofs, color<T> pixel_color, std::int32_t samples_per_pixel = 1) {
    const T s = T(1.0) / samples_per_pixel;
    const T r { s * pixel_color.x() };
    const T g { s * pixel_color.y() };
    const T b { s * pixel_color.z() };

    ofs << std::format("{} {} {}\n", scale<T, std::uint8_t>(r, 1.0F, 255U),
                       scale<T, std::uint8_t>(g, 1.0F, 255U),
                       scale<T, std::uint8_t>(b, 1.0F, 255U));
}
