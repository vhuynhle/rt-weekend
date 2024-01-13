#include "camera.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "vec3.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>

namespace {

constexpr double aspect_ratio { 16.0 / 9.0 };
constexpr std::size_t image_width { 1024};
const point3d camera_center { 0, 0, 0 };
constexpr double focal_length { 1.0 };
constexpr double viewport_height { 2.0 };
const std::int32_t samples_per_pixel { 10 };

}

int main() {
    camera<double> camera { aspect_ratio, image_width,     camera_center,
                            focal_length, viewport_height, samples_per_pixel };

    hittable_list<double> world;
    world.add(std::make_shared<sphere<double>>(point3d { 0, 0, -1 }, 0.5));      // small ball
    world.add(std::make_shared<sphere<double>>(point3d { 0, -100.5, -1 }, 100)); // earth

    camera.render(world);

    return 0;
}
