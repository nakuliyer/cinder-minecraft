#ifndef MINECRAFT_TERRAIN_GENERATOR_H
#define MINECRAFT_TERRAIN_GENERATOR_H

#include <FastNoiseLite.h>

#include "block_types.h"
#include "cinder/gl/gl.h"

namespace minecraft {

/// extendable class which implements a noise function to generate terrain
class TerrainGenerator {
 public:
  /// constructs a simple terrain generator
  ///
  /// \param min_height minimum height, i.e. sea level
  /// \param max_height maximum height, i.e. the height of the highest mountain
  /// \param variance low value (around 1.0f) for more flat terrain, high value
  /// (around 10.0f) for more varied terrain
  /// \param seed seed for Perlin noise
  TerrainGenerator(int min_height, int max_height, float variance, int seed);

  /// gets the block at (x, y, z)
  ///
  /// \param transform vector
  /// \return block type, or `BlockTypes::kNone` for air
  virtual BlockTypes GetBlockAt(const ci::vec3& transform);

 private:
  /// minimum height, i.e. sea level
  int min_height_;
  /// maximum height, i.e. the height of the highest mountain
  int max_height_;
  /// low value (around 1.0f) for more flat terrain, high value (around 10.0f)
  /// for more varied terrain
  float variance_;
  /// Perlin noise terrain generator
  FastNoiseLite noise_;

  /// Gets the terrain height
  ///
  /// \param x coordinate
  /// \param z coordinate
  /// \return some value between the constructor's passed `min_height` and
  /// `max_height`
  int GetTerrainHeight(int x, int z);
};

}  // namespace minecraft

#endif  // MINECRAFT_TERRAIN_GENERATOR_H
