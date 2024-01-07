#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <iostream>
#include <ostream>

template <std::unsigned_integral T>
constexpr T scale(T value, T old_max, T new_max)
{
    return std::clamp<T>(static_cast<double>(value) * new_max / old_max, 0, new_max);
}

int main()
{
    constexpr std::int32_t width { 256 };
    constexpr std::int32_t height { 256 };

    std::cout << std::format("P3\n{} {}\n255\n", width, height);
    for (std::int32_t row { 0 }; row < height; ++row) {
        std::clog << std::format("\rScanlines remaining: {} ", height - row) << &std::flush;
        for (std::int32_t col { 0 }; col < width; ++col) {
            const auto ir = scale<std::uint8_t>(col, width - 1, 255);
            const auto ig = scale<std::uint8_t>(row, height - 1, 255);
            const auto ib = 0;

            std::cout << std::format("{} {} {}\n", ir, ig, ib);
        }
    }

    std::cout << "\rDone.\n";

    return 0;
}
