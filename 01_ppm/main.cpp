#include "color.h"

#include <cstdint>
#include <format>
#include <iostream>
#include <ostream>

int main()
{
    constexpr std::int32_t width { 256 };
    constexpr std::int32_t height { 256 };

    std::cout << std::format("P3\n{} {}\n255\n", width, height);
    for (std::int32_t row { 0 }; row < height; ++row) {
        std::clog << std::format("\rScanlines remaining: {} ", height - row) << &std::flush;
        for (std::int32_t col { 0 }; col < width; ++col) {
            const auto pixel_color = colord { static_cast<double>(col) / (width - 1),
                static_cast<double>(row) / (height - 1),
                0 };
            write_color(std::cout, pixel_color);
        }
    }

    std::cout << "\rDone.\n";

    return 0;
}
