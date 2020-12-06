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
  /// angle difference allowed between displacement and the camera's forward
  /// vector for a block to be considered "in the direction" of the camera
  static const float kDirectionalAngleAllowance;

 public:
  /// initializes the terrain noise function and generates chunks adjacent to
  /// the player
  ///
  /// \param origin_position player's origin
  /// \param chunk_radius radius of each chunk
  /// \param render_radius radius to render blocks
  World(const ci::vec3& origin_position, size_t chunk_radius);

  /// renders the blocks in the player's chunk and all adjacent chunks if they
  /// are within rendering distance and in front of the player's field of view.
  /// if the player has moved between chunks, unloads the distance chunks from
  /// the blocks array and loads the new adjacent chunks
  ///
  /// \param player_transform the player's location
  /// \param camera_forward the camera's forward vector
  void Render(const ci::vec3& origin, const ci::vec3& forward,
              float field_of_view_angle, size_t render_radius) const;

  /// clips transform to the lattice block coordinate system and returns the
  /// block type at the transform from the current blocks array, or `kNone` if
  /// there is no block
  ///
  /// \param transform location
  /// \return block type
  BlockTypes GetBlockAt(const ci::vec3& transform);

  /// draws a stroked cube at the closest block in the direction of `forward`
  /// from `origin`
  ///
  /// \param origin a vector
  /// \param forward a vector
  void OutlineBlockInDirectionOf(const ci::vec3& origin,
                                 const ci::vec3& forward) const;

  /// deleted the closest block in the direction of `forward` from `origin`
  ///
  /// \param origin a vector
  /// \param forward a vector
  void DeleteBlockInDirectionOf(const ci::vec3& origin,
                                const ci::vec3& forward);

  /// \param old_chunk player's old chunk
  /// \param new_position player's new position
  /// \return true if and only if the player is outside the bounds of
  /// `old_chunk`
  bool HasMovedChunks(const std::vector<int>& old_chunk,
                      const ci::vec3& new_position) const;

  /// deletes far away (>1 chunk distance) blocks and pushes blocks to the
  /// blocks array that are adjacent to `new_chunk`. i.e. if the player has
  /// passed between chunks in the x direction, loads the adjacent chunks
  /// further away in the x direction in anticipation of movement there
  ///
  /// \param old_chunk player's old chunk
  /// \param new_chunk player's new chunk
  void MoveToChunk(const std::vector<int>& old_chunk,
                   const std::vector<int>& new_chunk);

  /// returns the chunk that a point is in
  ///
  /// \param point a point
  /// \return a chunk
  std::vector<int> GetChunk(const ci::vec3& point) const;

 private:
  /// radius of chunks
  size_t chunk_radius_;
  /// current set of blocks in the current chunk and all adjacent chunks
  std::vector<Block> blocks_;
  /// Perlin noise terrain generator
  FastNoiseLite noise_;

  /// initialization step, generates all chunks near the player at the start of
  /// game
  ///
  /// \param origin_chunk the player's initial chunk
  void InitializeAdjacentChunks(const std::vector<int>& origin_chunk);

  /// deletes all blocks that are more than one chunk away
  ///
  /// \param new_chunk the player's new chunk
  void DeleteDistanceChunks(const std::vector<int>& new_chunk);

  /// pushes blocks to the blocks array that are adjacent to the passed chunk.
  /// i.e. if the player has passed between chunks in the x direction, loads the
  /// adjacent chunks further away in the x direction in anticipation of
  /// movement there
  ///
  /// \param old_chunk the player's old chunk
  /// \param new_chunk the player's new chunk
  void LoadNextChunks(const std::vector<int>& old_chunk,
                      const std::vector<int>& new_chunk);

  /// generates a chunk a specified distance away from the player's chunk. for
  /// use-case simplicity, takes a reference chunk with the delta from that
  /// chunk.
  ///
  /// \param reference_chunk a reference chunk to calculate deltas from
  /// \param delta_x distance in chunk-distance
  /// \param delta_y distance in chunk-distance
  /// \param delta_z distance in chunk-distance
  void GenerateChunk(std::vector<int> old_chunk, int delta_x, int delta_y,
                     int delta_z);

  /// generates the block at the specified transform
  ///
  /// \param transform a location
  /// \return a block type, or `kNone`
  BlockTypes GenerateBlockAt(const ci::vec3& transform);

  /// gets the index in `blocks_` of the closest block in the direction of
  /// `forward` from `origin`, or -1 if there is no such block
  int GetBlockIndexInDirectionOf(const ci::vec3& origin,
                                 const ci::vec3& forward) const;

  /// whether or not a block should be rendered in a particular frame
  static bool IsWithinRenderDistance(const Block& block, const ci::vec3& origin,
                                     const ci::vec3& forward,
                                     float field_of_view_angle,
                                     size_t render_radius);

  /// angle between two vectors
  static float GetAngle(const ci::vec3& first, const ci::vec3& second);
};

}  // namespace minecraft

#endif  // MINE_CRAFTY_WORLD_MAP_H
