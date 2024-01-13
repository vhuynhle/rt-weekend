#pragma once

#include <concepts>
#include <limits>

template <std::floating_point T>
struct interval {
    T min;
    T max;

    interval()
        : interval { -std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity() } { }

    interval(T min_, T max_)
        : min { min_ }
        , max { max_ } { }

    interval(const interval&) = default;
    interval(interval&&) = default;
    interval& operator=(const interval&) = default;
    interval& operator=(interval&&) = default;
    ~interval() = default;

    bool contains(T x) const { return (x >= min) && (x <= max); }

    bool surrounds(T x) const { return (x > min) && (x < max); }

    static inline const interval empty { T(1), T(0) };
    static inline const interval universe {};
    static inline const interval nonnegative { T(0), std::numeric_limits<T>::infinity()};

};
