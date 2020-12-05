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
  GenerateAdjacentChunks();
}

void World::Render(const vec3& origin, const vec3& forward,
                   float field_of_view_angle) {
  for (const Block& block : blocks_) {
    if (IsWithinRenderDistance(block, origin, forward, field_of_view_angle)) {
      block.Render();
    }
  }
}

bool World::IsWithinRenderDistance(const Block& block, const vec3& origin,
                                   const vec3& forward,
                                   float field_of_view_angle) const {
  return distance(origin, block.GetCenter()) <= render_radius_ &&
         GetAngle(block.GetCenter() - origin, forward) <= field_of_view_angle;
}

bool World::HasMovedChunks(const vector<int>& old_chunk,
                           const vec3& new_position) const {
  return old_chunk != GetChunk(new_position);
}

void World::MoveToChunk(const vector<int>& old_chunk,
                        const vector<int>& new_chunk) {
  DeleteDistanceChunks(new_chunk);
  LoadNextChunks(old_chunk, new_chunk);
}

void World::DeleteDistanceChunks(const vector<int>& new_chunk) {
  size_t block_index = 0;
  while (block_index < blocks_.size()) {
    vector<int> block_chunk = GetChunk(blocks_[block_index].GetCenter());
    if (abs(block_chunk[0] - new_chunk[0]) > 1 &&
        abs(block_chunk[1] - new_chunk[1]) > 1 &&
        abs(block_chunk[2] - new_chunk[2]) > 1) {
      blocks_.erase(blocks_.begin() + block_index);
    } else {
      ++block_index;
    }
  }
}

void World::LoadNextChunks(const vector<int>& old_chunk,
                           const vector<int>& new_chunk) {
  if (old_chunk[0] != new_chunk[0]) {
    int direction = new_chunk[0] - old_chunk[0];
    for (int y = -1; y <= 1; ++y) {
      for (int z = -1; z <= 1; ++z) {
        GenerateChunk(vector<int>{new_chunk[0] + direction, new_chunk[1] + y,
                                  new_chunk[2] + z});
      }
    }
  } else if (old_chunk[1] != new_chunk[1]) {
    int direction = new_chunk[1] - old_chunk[1];
    for (int x = -1; x <= 1; ++x) {
      for (int z = -1; z <= 1; ++z) {
        GenerateChunk(vector<int>{new_chunk[0] + x, new_chunk[1] + direction,
                                  new_chunk[2] + z});
      }
    }
  } else if (old_chunk[2] != new_chunk[2]) {
    int direction = new_chunk[2] - old_chunk[2];
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        GenerateChunk(vector<int>{new_chunk[0] + x, new_chunk[1] + y,
                                  new_chunk[2] + direction});
      }
    }
  }
}

vector<int> World::GetChunk(const vec3& point) const {
  return vector<int>{int(point.x / (2.0f * chunk_radius_)),
                     int(point.y / (2.0f * chunk_radius_)),
                     int(point.z / (2.0f * chunk_radius_))};
}

void World::GenerateAdjacentChunks() {
  for (int x = -1; x < 2; ++x) {
    for (int y = -1; y < 2; ++y) {
      for (int z = -1; z < 2; ++z) {
        GenerateChunk(vector<int>{x, y, z});
      }
    }
  }
}

void World::GenerateChunk(vector<int> chunk) {
  int origin[] = {2 * chunk[0] * int(chunk_radius_),
                  2 * chunk[1] * int(chunk_radius_),
                  2 * chunk[2] * int(chunk_radius_)};
  int half_width = int(chunk_radius_);
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

float World::GetAngle(const vec3& first, const vec3& second) {
  return acos(dot(first, second) / (length(first) * length(second)));
}

}  // namespace minecraft