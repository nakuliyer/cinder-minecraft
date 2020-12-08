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
    if (transform.y == 0) {
      return BlockTypes::kGrass;
    } else if (transform.y == -1) {
      return BlockTypes::kDirt;
    }
    return BlockTypes::kNone;
  }
};

class TestableWorld : public World {
 public:
  TestableWorld(TerrainGenerator* terrainGenerator, const vec3& originPosition,
                size_t chunkRadius)
      : World(terrainGenerator, originPosition, chunkRadius) {
  }

  vector<Block> GetBlocks() {
    return blocks_;
  }
};

FlatTerrainGenerator flat_terrain_generator(-1, 1, 0, 0);
TestableWorld world(&flat_terrain_generator, vec3(0, 0, 0), 2);
vector<Block> blocks = world.GetBlocks();

TEST_CASE("Creating a terrain of chunks and getting blocks") {
  SECTION("Initial chunks loaded") {
    std::cout << blocks.size() << std::endl;
    int i = 0;
    for (const Block& block : blocks) {
      if (block.GetCenter().x == 6) {
        std::cout << i << " " << block.GetCenter() << std::endl;
        ++i;
      }
    }
  }
}

TEST_CASE("Chunk loading") {
  SECTION("Initial chunks have the right size") {
    REQUIRE(blocks.size() == 12 * 12 * 2);
  }
}