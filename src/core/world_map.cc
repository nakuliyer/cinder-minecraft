#include "core/world_map.h"

using ci::vec3;
using ci::gl::drawStrokedCube;
using glm::length;
using std::abs;

namespace minecraft {

WorldMap::WorldMap() {
  for (int x = -4; x <= 4; ++x) {
    for (int z = -4; z <= 4; ++z) {
      blocks_.emplace_back(x, -3, z);
    }
  }
}

void WorldMap::Render() {
  for (const vec3& block : blocks_) {
    drawStrokedCube(block, vec3(1, 1, 1));
  }
}

bool WorldMap::isOnLand(const vec3& transform) {
  for (const vec3& block : blocks_) {
    if (abs(transform.x - block.x) <= kBlockWidth / 2 &&
        abs(transform.z - block.z) <= kBlockWidth / 2 &&
        transform.y - block.y >
            kBlockWidth / 2 + kPlayerHeight - kLandingRoom &&
        transform.y - block.y < kBlockWidth / 2 + kPlayerHeight) {
      return true;
    }
  }
  return false;
}

}  // namespace minecraft