#include "core/world.h"

#include <catch2/catch.hpp>

#include "glm/gtx/string_cast.hpp"

using ci::vec3;
using minecraft::Block;
using minecraft::BlockTypes;
using minecraft::TerrainGenerator;
using minecraft::World;
using std::vector;

/// a semi-flat testing terrain
class TestingTerrainGenerator : public TerrainGenerator {
 public:
  TestingTerrainGenerator(int minHeight, int maxHeight, float variance,
                          int seed)
      : TerrainGenerator(minHeight, maxHeight, variance, seed) {
  }

  BlockTypes GetBlockAt(const ci::vec3& transform) {
    if (transform == vec3(7, 1, 0)) {
      return BlockTypes::kGrass;
    } else if (transform.y == 0) {
      return BlockTypes::kGrass;
    } else if (transform.y == -1) {
      return BlockTypes::kDirt;
    }
    return BlockTypes::kNone;
  }
};

/// the purpose of which is to expose certain protected variables of world
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

TestingTerrainGenerator testing_terrain_generator(-1, 1, 0, 0);
TestableWorld world(&testing_terrain_generator, vec3(0, 0, 0), 2);
vector<Block> blocks = world.GetBlocks();

TEST_CASE("Chunk loading") {
  SECTION("Initial chunks have the right size") {
    // the theoretical bounds of `blocks` is 12 * 12 * 12. this is derived from
    // the chunk size 2 provided to the constructor of `TestableWorld` - each
    // chunk has radius 2, so diameter 4, and we load all adjacent chunks,
    // yielding a total diameter of 12. in our case, only y=-1 and y=0 have
    // blocks, so this size should be corrected to 12 * 12 * 2.
    REQUIRE(blocks.size() == 12 * 12 * 2);
  }

  SECTION("Blocks are placed properly") {
    for (const Block& block : blocks) {
      vec3 center = block.GetCenter();
      // see the above comment for the usage of 6 - 6 is the radius of the
      // initialized chunks
      if (block.GetType() == BlockTypes::kNone) {
        FAIL("unexpected nonetype block");
      } else if (center.x < -6 || center.x >= 6 || center.z < -6 ||
                 center.z >= 6 || center.y < -1 || center.y > 0) {
        FAIL("a block was placed unexpectedly: " + glm::to_string(center));
      } else if ((center.y == 0 && block.GetType() == BlockTypes::kDirt) ||
                 (center.y == -1 && block.GetType() == BlockTypes::kGrass)) {
        FAIL("wrong type of block was placed");
      }
    }
  }

  SECTION("Not moving between chunks") {
    vector<int> v = world.GetChunk(vec3(-4, 0,0 ));
    for (int i : v) {
      std::cout << i << ", ";
    }
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(-1, 0, 1)) == false);
  }

  SECTION("Moving between chunks") {
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(3, 0, 0)) == true);
  }
}