#pragma once

#include "vec3.h"
#include <algorithm>
#include <cstdint>
#include <format>
#include <ostream>
#include <type_traits>

using color = vec3f;

template <typename T, typename U>
    requires std::is_arithmetic_v<T> && std::is_arithmetic_v<U>
constexpr U scale(T value, T old_max, U new_max)
{
    return std::clamp<T>(static_cast<double>(value) * new_max / old_max, 0, new_max);
}

inline void write_color(std::ostream& ofs, color pixel_color)
{
    ofs << std::format("{} {} {}\n",
        scale<float, std::uint8_t>(pixel_color.x(), 1.0F, 255U),
        scale<float, std::uint8_t>(pixel_color.y(), 1.0F, 255U),
        scale<float, std::uint8_t>(pixel_color.z(), 1.0F, 255U));
}
