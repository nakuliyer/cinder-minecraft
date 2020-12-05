#include "core/world.h"

#include <random>

using ci::vec2;
using ci::vec3;
using ci::gl::drawCube;
using glm::distance;
using glm::dot;
using glm::length;
using std::abs;
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

namespace minecraft {

const float World::kClosenessAngleCoefficient = 40.0f;
const float World::kClosenessPositionCoefficient = 1.0f;

World::World(size_t chunk_radius, size_t render_radius)
    : chunk_radius_(chunk_radius), render_radius_(render_radius) {
  random_device rd;
  mt19937 mt(rd());
  uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
  int seed = dist(mt);
  noise_ = FastNoiseLite(seed);
  noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  chunk_ = {0, 0, 0};
  GenerateAdjacentChunks();
}

void World::Render(const vec3& player_transform, const vec3& camera_forward,
                   float field_of_view_angle) {
  for (const Block& block : blocks_) {
    if (distance(player_transform, block.GetCenter()) <= kRenderRadius &&
        dot(block.GetCenter() - player_transform, camera_forward) > 0) {
      block.Render();
    }
  }
}

bool World::HasMovedChunks(const vec3& player_transform) const {
  vector<int> chunk = GetChunk(player_transform);
  return chunk_[0] != chunk[0] || chunk_[1] != chunk[1] ||
         chunk_[2] != chunk[2];
}

void World::MoveToChunk(const vec3& player_transform) {
  vector<int> chunk = GetChunk(player_transform);
  DeleteDistanceChunks(chunk);
  LoadNextChunk(chunk);
  chunk_ = {chunk[0], chunk[1], chunk[2]};
}

vector<int> World::GetChunk(const vec3& point) {
  return vector<int>{int(point.x / (2 * kGenerationRadius)),
                     int(point.y / (2 * kGenerationRadius)),
                     int(point.z / (2 * kGenerationRadius))};
}

void World::GenerateAdjacentChunks() {
  for (int x = -1; x < 2; ++x) {
    for (int y = -1; y < 2; ++y) {
      for (int z = -1; z < 2; ++z) {
        GenerateChunk(x, y, z);
      }
    }
  }
}

void World::GenerateChunk(int delta_x, int delta_y, int delta_z) {
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

void World::DeleteDistanceChunks(const vector<int>& chunk) {
  size_t block_index = 0;
  while (block_index < blocks_.size()) {
    vector<int> block_chunk = GetChunk(blocks_[block_index].GetCenter());
    if (abs(block_chunk[0] - chunk[0]) > 1 &&
        abs(block_chunk[1] - chunk[1]) > 1 &&
        abs(block_chunk[2] - chunk[2]) > 1) {
      blocks_.erase(blocks_.begin() + block_index);
    } else {
      ++block_index;
    }
  }
}

void World::LoadNextChunk(const vector<int>& chunk) {
  if (chunk[0] != chunk_[0]) {
    GenerateChunk(2 * (chunk[0] - chunk_[0]), 0, 0);
    GenerateChunk(2 * (chunk[0] - chunk_[0]), 0, -1);
    GenerateChunk(2 * (chunk[0] - chunk_[0]), 0, 1);
  } else if (chunk[1] != chunk_[1]) {
    GenerateChunk(0, 2 * (chunk[1] - chunk_[1]), 0);
    GenerateChunk(0, 2 * (chunk[1] - chunk_[1]), -1);
    GenerateChunk(0, 2 * (chunk[1] - chunk_[1]), 1);
  } else if (chunk[2] != chunk_[2]) {
    GenerateChunk(0, 0, 2 * (chunk[2] - chunk_[2]));
    GenerateChunk(-1, 0, 2 * (chunk[2] - chunk_[2]));
    GenerateChunk(1, 0, 2 * (chunk[2] - chunk_[2]));
  }
}

BlockTypes World::GetBlockAt(const vec3& transform) {
  for (const Block& block : blocks_) {
    if (abs(block.GetCenter().x - transform.x) <= 0.5 &&
        abs(block.GetCenter().y - transform.y) <= 0.5 &&
        abs(block.GetCenter().z - transform.z) <= 0.5) {
      return block.GetType();
    }
  }
  return BlockTypes::kNone;
}

BlockTypes World::GenerateBlockAt(const vec3& transform) {
  float height = noise_.GetNoise(round(transform.x), round(transform.z));
  if (int(round(transform.y)) <= int(height * 5.0f - 3.0f)) {
    return BlockTypes::kGrass;
  }
  return BlockTypes::kNone;
}

size_t World::GetClosestBlockIndex(const vec3& player_transform,
                                   const vec3& camera_forward) const {
  float min_closeness = FLT_MAX;
  size_t closest_block_index = 0;
  size_t index = 0;
  for (const Block& block : blocks_) {
    vec3 block_vector = block.GetCenter() - player_transform;
    float distance = length(block_vector);
    float delta_angle = acos(dot(camera_forward, block_vector / distance));
    float closeness = ComputeClosenessScore(delta_angle, distance);
    if (closeness < min_closeness) {
      min_closeness = closeness;
      closest_block_index = index;
    }
    ++index;
  }
  return closest_block_index;
}

vec3 World::GetClosestBlock(const vec3& player_transform,
                            const vec3& camera_forward) const {
  size_t closest_block = GetClosestBlockIndex(player_transform, camera_forward);
  return blocks_.at(closest_block).GetCenter();
}

void World::DeleteClosestBlock(const vec3& player_transform,
                               const vec3& camera_forward) {
  size_t closest_block = GetClosestBlockIndex(player_transform, camera_forward);
  blocks_.erase(blocks_.begin() + closest_block);
}

float World::ComputeClosenessScore(float delta_angle, float delta_position) {
  return kClosenessAngleCoefficient * delta_angle +
         kClosenessPositionCoefficient * delta_position;
}

}  // namespace minecraft