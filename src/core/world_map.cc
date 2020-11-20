#include "core/world_map.h"

using ci::vec3;
using ci::gl::drawCube;
using glm::length;
using std::abs;
using std::vector;

namespace minecraft {

WorldMap::WorldMap() {
  chunk_ = {0, 0, 0};
  GenerateAdjacentChunks();
  //  for (int x = -20; x <= 20; ++x) {
  //    for (int z = -20; z <= 20; ++z) {
  //      blocks_.emplace_back(vec3(x, -3, z));
  //    }
  //  }
  //  for (Block& block : blocks_) {
  //    block.SetUp();
  //  }
}

void WorldMap::Render(const vec3& player_transform,
                      const vec3& camera_forward) {
  int chunk_x = int(player_transform.x / kGenerationRadius);
  int chunk_y = int(player_transform.y / kGenerationRadius);
  int chunk_z = int(player_transform.z / kGenerationRadius);
  if (chunk_[0] != chunk_x || chunk_[1] != chunk_y || chunk_[2] != chunk_z) {
    chunk_ = {chunk_x, chunk_y, chunk_z};
    GenerateAdjacentChunks();
  } else {
  }
  for (const Block& block : blocks_) {
    if (abs(player_transform.x - block.GetCenter().x) <= 15 &&
        abs(player_transform.z - block.GetCenter().z) <= 15 &&
        ci::dot(block.GetCenter() - player_transform, camera_forward) > 0) {
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

void WorldMap::GenerateAdjacentChunks() {
  blocks_.clear();
  GenerateChunk(0, 0, 0);
  GenerateChunk(1, 0, 0);
  GenerateChunk(-1, 0, 0);
  GenerateChunk(0, 0, -1);
  GenerateChunk(0, 0, 1);

//  for (int x : {-1, 0, 1}) {
////    for (int y : {-1, 0, 1}) {
//      for (int z : {-1, 0, 1}) {
//        GenerateChunk(x, 0, z);
//      }
////    }
//  }
}

void WorldMap::GenerateChunk(int delta_x, int delta_y, int delta_z) {
  int origin[] = {(chunk_[0] + delta_x) * int(kGenerationRadius),
                  (chunk_[1] + delta_y) * int(kGenerationRadius),
                  (chunk_[2] + delta_z) * int(kGenerationRadius)};
  int half_width = int(kGenerationRadius / 2);
  for (int x = origin[0] - half_width; x < origin[0] + half_width; x++) {
    //for (int y = origin[1] - half_width; y < origin[1] + half_width; ++y) {
      for (int z = origin[2] - half_width; z < origin[2] + half_width; ++z) {
        if (GetBlockAt(vec3(x, -3, z)) != BlockTypes::kNone) {
          blocks_.emplace_back(vec3(x, -3, z));
        }
      }
    //}
  }
  for (Block& block : blocks_) {
    block.SetUp();
  }
}

BlockTypes WorldMap::GetBlockAt(const vec3& transform) {
  // TODO: here is where the noise function makes sense
  if (transform.y == -3) {
    return BlockTypes::kGrass;
  }
  return BlockTypes::kNone;
}

}  // namespace minecraft