#include "core/world_map.h"

using ci::vec3;
using ci::gl::drawCube;
using glm::length;
using std::abs;

namespace minecraft {

WorldMap::WorldMap() {
  for (int x = -12; x <= 20; ++x) {
    for (int z = -12; z <= 20; ++z) {
      blocks_.emplace_back(vec3(x, -3, z));
    }
  }
}

void WorldMap::Render() {
  int i = 0;
  for (const Block& block : blocks_) {
    block.Render();
    i++;
    if (i >= 120) {
      break;
    }
    // drawCube(block, vec3(1, 1, 1));
  }
}

bool WorldMap::isOnLand(const vec3& transform) {
  for (const Block& block : blocks_) {
    if (abs(transform.x - block.GetCenter().x) <= 0.5 &&
        abs(transform.z - block.GetCenter().z) <= 0.5 &&
        transform.y - block.GetCenter().y >
            0.5 + kPlayerHeight - kLandingRoom &&
        transform.y - block.GetCenter().y < 0.5 + kPlayerHeight) {
      return true;
    }
  }
  return false;
}

}  // namespace minecraft