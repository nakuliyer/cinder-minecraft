#include "core/terrain_generator.h"

#include <iostream>

namespace minecraft {

TerrainGenerator::TerrainGenerator(int min_height, int max_height,
                                   float variance, int seed)
    : min_height_(min_height), max_height_(max_height), variance_(variance) {
  noise_ = FastNoiseLite(seed);
  noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  std::cout << seed << std::endl;
}

int TerrainGenerator::GetTerrainHeight(int x, int z) {
  int height_delta = max_height_ - min_height_;
  float noise_function =
      noise_.GetNoise(round(x) * variance_, round(z) * variance_);
  return int(noise_function * float(height_delta) - float(min_height_));
}

}  // namespace minecraft