#include "core/world.h"

#include <random>

using ci::vec2;
using ci::vec3;
using ci::gl::drawCube;
using ci::gl::drawStrokedCube;
using glm::distance;
using glm::dot;
using glm::length;
using std::abs;
using std::mt19937;
using std::pair;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

namespace minecraft {

World::World(TerrainGenerator* terrain_generator,
             const ci::vec3& origin_position, size_t chunk_radius)
    : terrain_generator_(terrain_generator), chunk_radius_(chunk_radius) {
  InitializeAdjacentChunks(GetChunk(origin_position));
}

void World::Render(const vec3& origin, const vec3& forward,
                   float field_of_view_angle, size_t render_radius) const {
  for (const Block& block : blocks_) {
    if (IsWithinRenderDistance(block, origin, forward, field_of_view_angle,
                               render_radius)) {
      block.Render();
    }
  }
}

bool World::IsWithinRenderDistance(const Block& block, const vec3& origin,
                                   const vec3& forward,
                                   float field_of_view_angle,
                                   size_t render_radius) {
  return distance(origin, block.GetCenter()) <= render_radius &&
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
        GenerateChunk(new_chunk, direction, y, z);
      }
    }
  } else if (old_chunk[1] != new_chunk[1]) {
    int direction = new_chunk[1] - old_chunk[1];
    for (int x = -1; x <= 1; ++x) {
      for (int z = -1; z <= 1; ++z) {
        GenerateChunk(new_chunk, x, direction, z);
      }
    }
  } else if (old_chunk[2] != new_chunk[2]) {
    int direction = new_chunk[2] - old_chunk[2];
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        GenerateChunk(new_chunk, x, y, direction);
      }
    }
  }
}

vector<int> World::GetChunk(const vec3& point) const {
  return vector<int>{int(point.x / (2.0f * chunk_radius_)),
                     int(point.y / (2.0f * chunk_radius_)),
                     int(point.z / (2.0f * chunk_radius_))};
}

void World::InitializeAdjacentChunks(const vector<int>& origin_chunk) {
  for (int x = -1; x < 2; ++x) {
    for (int y = -1; y < 2; ++y) {
      for (int z = -1; z < 2; ++z) {
        GenerateChunk(origin_chunk, x, y, z);
      }
    }
  }
}

void World::GenerateChunk(vector<int> reference_chunk, int delta_x, int delta_y,
                          int delta_z) {
  int origin[] = {2 * (reference_chunk[0] + delta_x) * int(chunk_radius_),
                  2 * (reference_chunk[1] + delta_y) * int(chunk_radius_),
                  2 * (reference_chunk[2] + delta_z) * int(chunk_radius_)};
  int half_width = int(chunk_radius_);
  for (int x = origin[0] - half_width; x <= origin[0] + half_width; x++) {
    for (int y = origin[1] - half_width; y <= origin[1] + half_width; ++y) {
      for (int z = origin[2] - half_width; z <= origin[2] + half_width; ++z) {
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
  if (player_map_edits_.find(transform) != player_map_edits_.end()) {
    return player_map_edits_.at(transform);
  }
  return terrain_generator_->GetBlockAt(transform);
}

int World::GetBlockIndexInDirectionOf(const vec3& origin, const vec3& forward,
                                      float directional_angle_allowance) const {
  float min_distance = FLT_MAX;
  int closest_block = -1;
  size_t index = 0;
  for (const Block& block : blocks_) {
    vec3 displacement = block.GetCenter() - origin;
    if (GetAngle(forward, displacement) <= directional_angle_allowance &&
        length(displacement) < min_distance) {
      min_distance = length(displacement);
      closest_block = index;
    }
    ++index;
  }
  return closest_block;
}

void World::OutlineBlockInDirectionOf(const vec3& origin, const vec3& forward,
                                      float directional_angle_allowance) const {
  int closest_block =
      GetBlockIndexInDirectionOf(origin, forward, directional_angle_allowance);
  if (closest_block != -1) {
    drawStrokedCube(blocks_.at(closest_block).GetCenter(), vec3(1, 1, 1));
  }
}

void World::DeleteBlockInDirectionOf(const vec3& origin, const vec3& forward,
                                     float directional_angle_allowance) {
  size_t closest_block =
      GetBlockIndexInDirectionOf(origin, forward, directional_angle_allowance);
  if (closest_block != -1) {
    player_map_edits_.insert(pair<vec3, BlockTypes>(
        blocks_.at(closest_block).GetCenter(), BlockTypes::kNone));
    blocks_.erase(blocks_.begin() + closest_block);
  }
}

void World::CreateBlockInDirectionOf(const vec3& origin, const vec3& forward,
                                     const BlockTypes& block_type,
                                     float directional_angle_allowance) {
  size_t closest_block_index =
      GetBlockIndexInDirectionOf(origin, forward, directional_angle_allowance);
  if (closest_block_index == -1) {
    return;
  }
  vec3 closest_block = blocks_.at(closest_block_index).GetCenter();
  vec3 displacement = origin - closest_block;
  vec3 desired_hit_box =
      FindAxisAlignedUnitVector(displacement) + closest_block;
  if (GetBlockAt(desired_hit_box) == kNone) {
    player_map_edits_.insert(
        pair<vec3, BlockTypes>(desired_hit_box, block_type));
    blocks_.emplace_back(block_type, desired_hit_box);
  }
}

float World::GetAngle(const vec3& first, const vec3& second) {
  return acos(dot(first, second) / (length(first) * length(second)));
}

vec3 World::FindAxisAlignedUnitVector(const vec3& vector) {
  if (abs(vector.x) >= abs(vector.y) && abs(vector.x) >= abs(vector.z)) {
    return vec3(copysign(1, vector.x), 0, 0);
  } else if (abs(vector.y) >= abs(vector.x) && abs(vector.y) >= abs(vector.z)) {
    return vec3(0, copysign(1, vector.y), 0);
  } else {
    return vec3(0, 0, copysign(1, vector.z));
  }
}

}  // namespace minecraft