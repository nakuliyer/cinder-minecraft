#include "core/world_map.h"

using ci::vec3;
using ci::gl::drawCube;
using glm::distance;
using glm::dot;
using glm::length;
using std::abs;
using std::vector;

namespace minecraft {

WorldMap::WorldMap() {
  chunk_ = {0, 0, 0};
  GenerateAdjacentChunks();
  noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

void WorldMap::Render(const vec3& player_transform,
                      const vec3& camera_forward) {
  vector<int> chunk = GetChunk(player_transform);
  if (chunk_[0] != chunk[0] || chunk_[1] != chunk[1] || chunk_[2] != chunk[2]) {
    DeleteDistanceChunks(chunk);
    LoadNextChunk(chunk);
    chunk_ = {chunk[0], chunk[1], chunk[2]};
  }
  for (const Block& block : blocks_) {
    if (distance(player_transform, block.GetCenter()) <= kRenderRadius &&
        dot(block.GetCenter() - player_transform, camera_forward) > 0) {
      block.Render();
    }
  }
}

vector<int> WorldMap::GetChunk(const vec3& point) {
  return vector<int>{int(point.x / (2 * kGenerationRadius)),
                     int(point.y / (2 * kGenerationRadius)),
                     int(point.z / (2 * kGenerationRadius))};
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
  int origin[] = {2 * (chunk_[0] + delta_x) * int(kGenerationRadius),
                  2 * (chunk_[1] + delta_y) * int(kGenerationRadius),
                  2 * (chunk_[2] + delta_z) * int(kGenerationRadius)};
  int half_width = int(kGenerationRadius);
  for (int x = origin[0] - half_width; x < origin[0] + half_width; x++) {
    for (int y = origin[1] - half_width; y < origin[1] + half_width; ++y) {
      for (int z = origin[2] - half_width; z < origin[2] + half_width; ++z) {
        BlockTypes block_type = GenerateBlockAt(vec3(x, y, z));
        if (block_type != BlockTypes::kNone) {
          blocks_.emplace_back(block_type, vec3(x, y, z));
        }
      }
    }
  }
}

void WorldMap::DeleteDistanceChunks(const vector<int>& chunk) {
  size_t block_index = 0;
  while (block_index < blocks_.size()) {
    vector<int> block_chunk = GetChunk(blocks_[block_index].GetCenter());
    if (abs(block_chunk[0] - chunk[0]) > 1 &&
        abs(block_chunk[2] - chunk[2]) > 1) {
      blocks_.erase(blocks_.begin() + block_index);
    } else {
      ++block_index;
    }
  }
}

void WorldMap::LoadNextChunk(const vector<int>& chunk) {
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
}

BlockTypes WorldMap::GetBlockAt(const vec3& transform) {
  for (const Block& block : blocks_) {
    if (abs(block.GetCenter().x - transform.x) <= 0.5 &&
        abs(block.GetCenter().y - transform.y) <= 0.5 &&
        abs(block.GetCenter().z - transform.z) <= 0.5) {
      return block.GetType();
    }
  }
  return BlockTypes::kNone;
}

BlockTypes WorldMap::GenerateBlockAt(const vec3& transform) {
  float height =
      noise_.GetNoise(round(transform.x), round(transform.z)) * 5.0f - 3.0f;
  if (int(round(transform.y)) <= int(height)) {
    return BlockTypes::kGrass;
  }
  return BlockTypes::kNone;
}

}  // namespace minecraft