#include "core/world.h"

#include <catch2/catch.hpp>

#include "glm/gtx/string_cast.hpp"

using ci::vec3;
using minecraft::Block;
using minecraft::BlockTypes;
using minecraft::TerrainGenerator;
using minecraft::World;
using std::to_string;
using std::unordered_map;
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

  unordered_map<ci::vec3, BlockTypes, BlockHasher> GetPlayerMapEdits() {
    return player_map_edits_;
  }
};

TestingTerrainGenerator testing_terrain_generator(-1, 1, 0, 0);

TEST_CASE("Chunk loading") {
  TestableWorld world(&testing_terrain_generator, vec3(0, 0, 0), 2);

  SECTION("Initial chunks have the right size") {
    // the theoretical bounds of `blocks` is 12 * 12 * 12. this is derived from
    // the chunk size 2 provided to the constructor of `TestableWorld` - each
    // chunk has radius 2, so diameter 4, and we load all adjacent chunks,
    // yielding a total diameter of 12. in our case, only y=-1 and y=0 have
    // blocks, so this size should be corrected to 12 * 12 * 2.
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2);
  }

  SECTION("Blocks are placed properly") {
    for (const Block& block : world.GetBlocks()) {
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
}

TEST_CASE("Chunk movement detection") {
  TestableWorld world(&testing_terrain_generator, vec3(0, 0, 0), 2);

  SECTION("Detection of not moving between chunks") {
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(-1, 0, 1)) == false);
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(-2, 0, 1)) == false);
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(-1, 0, -2)) == false);
  }

  SECTION("Detection of moving between chunks") {
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(-2, 0, 2)) == true);
    REQUIRE(world.HasMovedChunks({0, 0, 0}, vec3(2, 0, 1)) == true);
  }
}

TEST_CASE("Chunk movement") {
  TestableWorld world(&testing_terrain_generator, vec3(0, 0, 0), 2);

  world.MoveToChunk({0, 0, 0}, {1, 0, 0});

  SECTION("Loads proper size") {
    // now, the world should have the abnormal (7, 1, 0) block
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2 + 1);
  }

  SECTION("Places blocks correctly") {
    // simplified from the `Chunk loading` example, with the assumption that
    // that test case passed
    bool found_abnormal_block = false;
    for (const Block& block : world.GetBlocks()) {
      vec3 center = block.GetCenter();
      if (center.x < -2 || center.x >= 10 || center.z < -6 || center.z >= 6) {
        FAIL("the old chunk is still there; a block was placed unexpectedly: " +
             glm::to_string(center));
      } else if (center == vec3(7, 1, 0)) {
        found_abnormal_block = true;
      }
    }
    REQUIRE(found_abnormal_block);
  }
}

TEST_CASE("Deleting a block from a direction") {
  TestableWorld world(&testing_terrain_generator, vec3(0, 0, 0), 2);

  SECTION("Not looking at any block") {
    REQUIRE(world.DeleteBlockInDirectionOf(vec3(0, 1, 0), vec3(1, 0, 0), 0.1) ==
            BlockTypes::kNone);
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2);
    REQUIRE(world.GetPlayerMapEdits().size() == 0);
  }

  SECTION("Looking at a block") {
    REQUIRE(world.DeleteBlockInDirectionOf(vec3(0, 1, 0),
                                           vec3(0.707, -0.707, 0),
                                           0.1) == BlockTypes::kGrass);
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2 - 1);
    for (const Block& block : world.GetBlocks()) {
      if (block.GetCenter() == vec3(1, 0, 0)) {
        FAIL("didn't delete the expected block");
      }
    }
  }

  SECTION("Deleted block from world edit map") {
    world.DeleteBlockInDirectionOf(vec3(0, 1, 0), vec3(0.707, -0.707, 0), 0.1);
    REQUIRE(world.GetPlayerMapEdits().size() == 1);
    REQUIRE(world.GetPlayerMapEdits().at(vec3(1, 0, 0)) == BlockTypes::kNone);
  }
}

TEST_CASE("Creating a block from a direction") {
  TestableWorld world(&testing_terrain_generator, vec3(4, 0, 0), 2);

  SECTION("Not looking at any block") {
    REQUIRE_FALSE(world.CreateBlockInDirectionOf(
        vec3(4, 1, 0), vec3(0.707, 0.707, 0), BlockTypes::kDirt, 0.1));
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2 + 1);
    REQUIRE(world.GetPlayerMapEdits().size() == 0);
  }

  SECTION("Looking at a block from front/back") {
    REQUIRE(world.CreateBlockInDirectionOf(vec3(4, 1, 0), vec3(1, 0, 0),
                                           BlockTypes::kDirt, 0.1));
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2 + 2);
    bool created = false;
    for (const Block& block : world.GetBlocks()) {
      if (block.GetCenter() == vec3(6, 1, 0) &&
          block.GetType() == BlockTypes::kDirt) {
        created = true;
      }
    }
    REQUIRE(created);
  }

  SECTION("Looking at a block from side") {
    REQUIRE(world.CreateBlockInDirectionOf(vec3(7, 1, 2), vec3(0, 0, -1),
                                           BlockTypes::kDirt, 0.1));
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2 + 2);
    bool created = false;
    for (const Block& block : world.GetBlocks()) {
      if (block.GetCenter() == vec3(7, 1, 1) &&
          block.GetType() == BlockTypes::kDirt) {
        created = true;
      }
    }
    REQUIRE(created);
  }

  SECTION("Looking at a block from top/bottom") {
    REQUIRE(world.CreateBlockInDirectionOf(vec3(7, 3, 0), vec3(0, -1, 0),
                                           BlockTypes::kDirt, 0.1));
    REQUIRE(world.GetBlocks().size() == 12 * 12 * 2 + 2);
    bool created = false;
    for (const Block& block : world.GetBlocks()) {
      if (block.GetCenter() == vec3(7, 2, 0) &&
          block.GetType() == BlockTypes::kDirt) {
        created = true;
      }
    }
    REQUIRE(created);
  }

  SECTION("Added block to world edit map") {
    world.CreateBlockInDirectionOf(vec3(7, 3, 0), vec3(0, -1, 0),
                                   BlockTypes::kDirt, 0.1);
    REQUIRE(world.GetPlayerMapEdits().size() == 1);
    REQUIRE(world.GetPlayerMapEdits().at(vec3(7, 2, 0)) == BlockTypes::kDirt);
  }
}

TEST_CASE("World edits is preserve through chunk movement") {
  TestableWorld world(&testing_terrain_generator, vec3(4, 0, 0), 2);
  world.DeleteBlockInDirectionOf(vec3(0, 1, 0), vec3(0.707, -0.707, 0), 0.1);
  world.CreateBlockInDirectionOf(vec3(3, 3, 0), vec3(0, -1, 0),
                                 BlockTypes::kStone, 0.1);

  REQUIRE(world.GetPlayerMapEdits().size() == 2);

  SECTION("Allows chunk unloading and the returning to edited blocks") {
    world.MoveToChunk({1, 0, 0}, {2, 0, 0});
    world.MoveToChunk({2, 0, 0}, {3, 0, 0});
    world.MoveToChunk({3, 0, 0}, {4, 0, 0});  // simulating movement in +x
    world.MoveToChunk({4, 0, 0}, {5, 0, 0});  // assume chunk {1, 0, 0} is
                                              // deleted by now based on the
                                              // success of previous test cases
    world.MoveToChunk({5, 0, 0}, {4, 0, 0});
    world.MoveToChunk({4, 0, 0}, {3, 0, 0});
    world.MoveToChunk({3, 0, 0}, {2, 0, 0});  // this loads chunk {1, 0, 0}
    bool found_created_block = false;
    for (const Block& block : world.GetBlocks()) {
      if (block.GetCenter() == vec3(1, 0, 0)) {
        FAIL("the deleted block resurfaced as a " + to_string(block.GetType()));
      }
      if (block.GetCenter() == vec3(3, 1, 0) &&
          block.GetType() == BlockTypes::kStone) {
        found_created_block = true;
      }
    }
    REQUIRE(found_created_block);
  }
}

// note: if `CreateBlockInDirectionOf` and `DeleteBlockInDirectionOf` pass, the
// underlying implementation of the more difficult-to-test
// `OutlineBlockInDirectionOf` is assumed to work. this should be manually
// tested in gameplay.