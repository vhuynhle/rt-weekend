#pragma once

#include "hittable.h"
#include "ray.hpp"

#include <algorithm>
#include <concepts>
#include <memory>
#include <optional>
#include <vector>

template <std::floating_point T>
class hittable_list : public hittable<T> {
public:
    hittable_list() = default;
    void add(std::shared_ptr<hittable<T>> obj) { objects.push_back(std::move(obj)); }
    void clear() { objects.clear(); }

    std::optional<hit_record<T>> hit(const ray<T>& r, T ray_tmin, T ray_tmax) const override {
        std::optional<hit_record<T>> record { std::nullopt };

        for (const auto& obj : objects) {
            auto maybe_hit = obj->hit(r, ray_tmin, ray_tmax);
            if (maybe_hit.has_value() && maybe_hit->t < ray_tmax) {
                record = maybe_hit;
                ray_tmax = record->t;
            }
        }

        return record;
    }

private:
    std::vector<std::shared_ptr<hittable<T>>> objects {};
};
