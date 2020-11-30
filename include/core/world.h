#ifndef MINECRAFT_WORLD_H
#define MINECRAFT_WORLD_H

#include <FastNoiseLite.h>

#include <vector>

#include "cinder/gl/gl.h"
#include "core/block.h"
#include "core/block_types.h"

namespace minecraft {

/// cinder-compatible world
class World {
  /// x, y, and z width of a chunk
  static const size_t kGenerationRadius = 3;
  /// maximum distance from camera allowed to render a block
  static const size_t kRenderRadius = 15;
  /// importance of angle difference on the closeness score between player and
  /// block
  static const float kClosenessAngleCoefficient;
  /// importance of position difference on the closeness score between player
  /// and block
  static const float kClosenessPositionCoefficient;

 public:
  /// initializes the Perlin noise and generates chunks adjacent to the player
  World();

  /// renders the blocks in the player's chunk and all adjacent chunks if they
  /// are within rendering distance and in front of the player's field of view.
  /// if the player has moved between chunks, unloads the distance chunks from
  /// the blocks array and loads the new adjacent chunks
  ///
  /// \param player_transform the player's location
  /// \param camera_forward the camera's forward vector
  void Render(const ci::vec3& player_transform, const ci::vec3& camera_forward);

  /// clips transform to the lattice block coordinate system and returns the
  /// block type at the transform from the current blocks array, or `kNone` if
  /// there is no block
  ///
  /// \param transform location
  /// \return block type
  BlockTypes GetBlockAt(const ci::vec3& transform);

 private:
  /// current chunk
  std::vector<int> chunk_;
  /// current set of blocks in the current chunk and all adjacent chunks
  std::vector<Block> blocks_;
  /// Perlin noise terrain generator
  FastNoiseLite noise_;

  /// deletes all blocks that are more than one chunk away
  void DeleteDistanceChunks(const std::vector<int>& chunk);

  /// pushes blocks to the blocks array that are adjacent to the passed chunk.
  /// i.e. if the player has passed between chunks in the x direction, loads the
  /// adjacent chunks further away in the x direction in anticipation of
  /// movement there
  ///
  /// \param chunk the players new chunk
  void LoadNextChunk(const std::vector<int>& chunk);

  /// generates the block at the specified transform
  ///
  /// \param transform a location
  /// \return a block type, or `kNone`
  BlockTypes GenerateBlockAt(const ci::vec3& transform);

  /// initialization step, generates all chunks near the player at the start of
  /// game
  void GenerateAdjacentChunks();

  /// generates a chunk a specified distance away from the player's chunk
  ///
  /// \param delta_x distance in chunk-distance
  /// \param delta_y distance in chunk-distance
  /// \param delta_z distance in chunk-distance
  void GenerateChunk(int delta_x, int delta_y, int delta_z);

  ci::vec3 FindBlockPointedAt(const ci::vec3& player_transform,
                              const ci::vec3& camera_forward) const;

  /// returns the chunk that a point is in
  ///
  /// \param point a point
  /// \return a chunk
  static std::vector<int> GetChunk(const ci::vec3& point);

  static float ComputeClosenessScore(float delta_angle, float delta_position);
};

}  // namespace minecraft

#endif  // MINE_CRAFTY_WORLD_MAP_H