#ifndef MINECRAFT_GAME_ENGINE_H
#define MINECRAFT_GAME_ENGINE_H

#include <cinder/Camera.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>

#include <map>
#include <string>
#include <vector>

#include "core/camera.h"
#include "core/terrain_generator.h"
#include "core/world.h"

namespace minecraft {

/// TODO: move all these inside the class
static const float kWindowSize = 575.0f;
static const float kCentralPartition = 0.5f;

static const float kMoveDistance = 0.8f;
static const float kJumpForce = 0.3f;       // velocity upwards on jump
static const float kGravityForce = 0.03f;   // acceleration downwards
static const float kRotationSpeed = 0.05f;  // in radians

static const float kPlayerHeight = 1.9f;

class MinecraftApp : public ci::app::App {
  /// window size
  static const float kWindowSize;
  /// the percent of the screen's width/height in the middle where mouse
  /// position does not pan the camera
  static const float kCentralPartition;
  /// distance to move when any move key is pressed
  static const float kMoveDistance;
  /// acceleration upwards on jump
  static const float kJumpForce;
  /// acceleration downwards
  static const float kGravityForce;
  /// rotation speed when mouse pans
  static const float kRotationSpeed;
  /// the player's height, used to break fall on the group
  static const float kPlayerHeight;
  /// attributes for the UI text
  static const ci::vec2 kLeftUITextPosition;
  /// attributes for the UI text
  static const ci::vec2 kRightUITextPosition;
  /// attributes for the UI text
  static const ci::Color kUITextColor;
  /// attributes for the UI text
  static const ci::Font kUITextFont;
  /// attributes for the UI text
  static const float kUITextSpacing;
  /// field of view angle, blocks are not rendered outside this
  static const float kFieldOfViewAngle;
  /// radius of the chunks, see `world.h` for the usage
  static const size_t kChunkRadius;
  /// maximum distance from player to render blocks in
  static const size_t kRenderRadius;
  /// starting position
  static const ci::vec3 kPlayerStartingPosition;
  /// minimum height of terrain, i.e. sea level
  static const int kMinTerrainHeight;
  /// maximum height of terrain
  static const int kMaxTerrainHeight;
  /// variance of terrain, see `terrain_generator.h` for usage
  static const float kTerrainVariance;
  /// maximum seed length
  static const size_t kMaxSeedLength;
  /// angle difference (in radians) allowed between displacement and the
  /// camera's forward vector for a block to be considered "in the direction" of
  /// the camera
  static const float kDirectionalAngleAllowance;

 public:
  MinecraftApp();
  void draw() override;
  void update() override;
  void keyDown(ci::app::KeyEvent e) override;

 private:
  int seed_;
  Camera camera_;
  TerrainGenerator terrain_generator_;
  World world_;
  std::vector<int> current_chunk_;
  std::map<BlockTypes, size_t> inventory_;
  BlockTypes current_placing_type_;

  void DrawUI();
  void MoveIfPossible(float delta_x, float delta_y);
  bool BlockExistsAt(float delta_x, float delta_y, float delta_z);
  void PanScreen(const ci::vec2& mouse_point);
  static bool IsBoundedBy(const ci::vec2& point, float x_min, float x_max,
                          float y_min, float y_max);
};

}  // namespace minecraft

#endif  // MINECRAFT_GAME_ENGINE_H
