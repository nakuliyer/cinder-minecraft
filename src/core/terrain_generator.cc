#include "core/terrain_generator.h"

#include <iostream>

using ci::vec3;

namespace minecraft {

TerrainGenerator::TerrainGenerator(int min_height, int max_height,
                                   float variance, int seed)
    : min_height_(min_height), max_height_(max_height), variance_(variance) {
  noise_ = FastNoiseLite(seed);
  noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

BlockTypes TerrainGenerator::GetBlockAt(const vec3& transform) {
  int height = GetTerrainHeight(transform.x, transform.z);
  int lattice_y = int(round(transform.y));
  if (lattice_y == height) {
    return BlockTypes::kGrass;
  } else if (0 <= lattice_y && lattice_y < height) {
    return BlockTypes::kDirt;
  } else if (lattice_y < height) {
    return BlockTypes::kStone;
  }
  return BlockTypes::kNone;
}

int TerrainGenerator::GetTerrainHeight(int x, int z) {
  int height_delta = max_height_ - min_height_;
  float noise_function =
      noise_.GetNoise(round(x) * variance_, round(z) * variance_);
  return int(noise_function * float(height_delta) - float(min_height_));
}

}  // namespace minecraft