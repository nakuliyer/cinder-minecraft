#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

// note, alongside the notes in the specific test files: GameEngine is not
// tested as it connects directly with cinder visualization functions and mostly
// acts as a wrapper for the other classes. It should be tested in gameplay.
//
// Block and Texture can be tested through the `minecraft-manual-test` cinder
// app. This will replace all textures with test texture; in this the faces are
// number from 1 to 6 in the order TOP, FRONT, RIGHT, BACK, LEFT, BOTTOM. All
// number should be oriented properly.
//
// TerrainGenerator, being essentially a wrapper around FastNoiseLite, is not
// explicitly test (though extended in `world_test.cc`). It should be observed
// in gameplay.