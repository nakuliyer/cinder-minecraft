#include "core/world.h"

#include <catch2/catch.hpp>

using ci::vec3;
using minecraft::Block;
using minecraft::BlockTypes;
using minecraft::TerrainGenerator;
using minecraft::World;
using std::vector;

class FlatTerrainGenerator : public TerrainGenerator {
 public:
  FlatTerrainGenerator(int minHeight, int maxHeight, float variance, int seed)
      : TerrainGenerator(minHeight, maxHeight, variance, seed) {
  }

  BlockTypes GetBlockAt(const ci::vec3& transform) {
    if (transform.y == 7) {
      return BlockTypes::kGrass;
    } else if (transform.y == 6) {
      return BlockTypes::kDirt;
    }
    return BlockTypes::kNone;
  }
};

class TestableWorld : public World {
 public:
  TestableWorld(const TerrainGenerator& terrainGenerator,
                const vec3& originPosition, size_t chunkRadius)
      : World(terrainGenerator, originPosition, chunkRadius) {
  }

  vector<Block> GetBlocks() {
    return blocks_;
  }
};

TEST_CASE("Creating a terrain of chunks and getting blocks") {
  FlatTerrainGenerator flat_terrain_generator(-1, 1, 0, 0);
  TestableWorld world(flat_terrain_generator, vec3(0, 0, 0), 2);

  SECTION("Initial chunks loaded") {
    vector<Block> blocks = world.GetBlocks();
    std::cout << blocks.size() << std::endl;
    for (const Block& block : blocks) {
      std::cout << block.GetCenter() << std::endl;
    }
  }
}