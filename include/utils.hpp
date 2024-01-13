#pragma once

#include <concepts>
#include <random>

template <std::floating_point T>
T get_random(T min = T(0), T max = T(1)) {
    static std::random_device rd;
    static std::uniform_real_distribution<T> dist { min, max };
    static std::mt19937_64 generator { rd() };
    return dist(generator);
}
