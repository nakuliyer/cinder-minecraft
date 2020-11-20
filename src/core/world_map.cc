#include "core/world_map.h"

using ci::vec3;
using ci::gl::drawCube;
using glm::length;
using std::abs;
using std::vector;

namespace minecraft {

WorldMap::WorldMap() {
//  for (int x = -100; x <= 100; ++x) {
//    for (int z = -100; z <= 100; ++z) {
//      world_.emplace_back(vec3(x, -3, z));
//    }
//  }
  for (int x = -20; x <= 20; ++x) {
    for (int z = -20; z <= 20; ++z) {
      blocks_.emplace_back(vec3(x, -3, z));
    }
  }
  for (Block& block : blocks_) {
    block.SetUp();
  }
}

void WorldMap::Render(const vec3& camera_transform,
                      const vec3& camera_forward) {
  for (const Block& block : blocks_) {
    if (abs(camera_transform.x - block.GetCenter().x) <= 15 &&
        abs(camera_transform.z - block.GetCenter().z) <= 15 &&
        ci::dot(block.GetCenter() - camera_transform, camera_forward) > 0) {
      block.Render();
    }
  }
}

bool WorldMap::IsOnLand(const vec3& transform) {
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

BlockTypes WorldMap::GetBlockAt(const vec3& transform) {
  // TODO: here is where the noise function makes sense
  if (transform.y == -3) {
    return BlockTypes::kGrass;
  }
  return BlockTypes::kNone;
}

}  // namespace minecraft