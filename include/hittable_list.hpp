#pragma once

#include "hittable.hpp"
#include "interval.hpp"
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

    std::optional<hit_record<T>> hit(const ray<T>& r, interval<T> rayt) const override {
        std::optional<hit_record<T>> record { std::nullopt };

        T tmax { rayt.max };
        for (const auto& obj : objects) {
            std::optional<hit_record<T>> maybe_hit { obj->hit(r, rayt) };
            if (maybe_hit.has_value() && maybe_hit->t < tmax) {
                record = maybe_hit;
                tmax = record->t;
            }
        }

        return record;
    }

private:
    std::vector<std::shared_ptr<hittable<T>>> objects {};
};
