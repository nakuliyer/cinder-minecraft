#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include <vector>
#include <FastNoiseLite.h>

#include "cinder/gl/gl.h"
#include "core/block.h"
#include "core/block_types.h"

namespace minecraft {

class World {
  static const size_t kGenerationRadius = 3;
  static const size_t kRenderRadius = 15;

 public:
  World();
  void Render(const ci::vec3& camera_transform_,
              const ci::vec3& camera_forward);
  BlockTypes GetBlockAt(const ci::vec3& transform);

 private:
  std::vector<int> chunk_;
  std::vector<Block> blocks_;
  FastNoiseLite noise_;

  void DeleteDistanceChunks(const std::vector<int>& chunk);
  void LoadNextChunk(const std::vector<int>& chunk);
  BlockTypes GenerateBlockAt(const ci::vec3& transform);
  static std::vector<int> GetChunk(const ci::vec3& point);
  void GenerateAdjacentChunks();
  void GenerateChunk(int delta_x, int delta_y, int delta_z);
};

}  // namespace minecraft

#endif  // MINE_CRAFTY_WORLD_MAP_H
