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
  noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      std::cout << "(" << x << ", " << y << ") -> " << noise_.GetNoise(float(x), float(y)) << std::endl;
    }
  }
}

void WorldMap::Render(const vec3& player_transform,
                      const vec3& camera_forward) {
  vector<int> chunk = GetChunk(player_transform);
  if (chunk_[0] != chunk[0] || chunk_[1] != chunk[1] || chunk_[2] != chunk[2]) {
    std::cout << "moved chunks" << std::endl;
    size_t i = 0;
    while (i < blocks_.size()) {
      vector<int> block_chunk = GetChunk(blocks_[i].GetCenter());
      if (abs(block_chunk[0] - chunk[0]) > 1 ||
          abs(block_chunk[2] - chunk[2]) > 1) {
        blocks_.erase(blocks_.begin() + i);
      } else {
        i++;
      }
    }
    if (chunk[0] != chunk_[0]) {
      GenerateChunk(2 * (chunk[0] - chunk_[0]), 0, 0);
      GenerateChunk(2 * (chunk[0] - chunk_[0]), 0, -1);
      GenerateChunk(2 * (chunk[0] - chunk_[0]), 0, 1);
    } else if (chunk[1] != chunk_[1]) {
      GenerateChunk(0, 2 * (chunk[1] - chunk_[1]), 0);
    } else if (chunk[2] != chunk_[2]) {
      GenerateChunk(0, 0, 2 * (chunk[2] - chunk_[2]));
      GenerateChunk(-1, 0, 2 * (chunk[2] - chunk_[2]));
      GenerateChunk(1, 0, 2 * (chunk[2] - chunk_[2]));
    }
    chunk_ = {chunk[0], chunk[1], chunk[2]};
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

vector<int> WorldMap::GetChunk(const vec3& point) {
  return vector<int>{int(point.x / kGenerationRadius),
                     int(point.y / kGenerationRadius),
                     int(point.z / kGenerationRadius)};
}

void WorldMap::GenerateAdjacentChunks() {
  blocks_.clear();
  for (int x = -1; x < 2; ++x) {
    for (int z = -1; z < 2; ++z) {
      GenerateChunk(x, 0, z);
    }
  }
}

void WorldMap::GenerateChunk(int delta_x, int delta_y, int delta_z) {
  int origin[] = {(chunk_[0] + delta_x) * int(kGenerationRadius),
                  (chunk_[1] + delta_y) * int(kGenerationRadius),
                  (chunk_[2] + delta_z) * int(kGenerationRadius)};
  int half_width = int(kGenerationRadius / 2);
  for (int x = origin[0] - half_width; x < origin[0] + half_width; x++) {
    for (int y = origin[1] - half_width; y < origin[1] + half_width; ++y) {
      for (int z = origin[2] - half_width; z < origin[2] + half_width; ++z) {
        if (GetBlockAt(vec3(x, y, z)) != BlockTypes::kNone) {
          blocks_.emplace_back(vec3(x, y, z));
          blocks_.back().SetUp();
        }
      }
    }
  }
}

BlockTypes WorldMap::GetBlockAt(const vec3& transform) {
  // TODO: here is where the noise function makes sense
  if (transform.y == int(noise_.GetNoise(transform.x, transform.z) * 5) - 3) {
    return BlockTypes::kGrass;
  }
  return BlockTypes::kNone;
}

}  // namespace minecraft