#ifndef MINECRAFT_GAME_ENGINE_H
#define MINECRAFT_GAME_ENGINE_H

#include <string>
#include <vector>

#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "core/camera.h"
#include "core/world.h"
#include "cinder/gl/Fbo.h"

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
  static const ci::vec2 kCoordinatesTextTopLeft;
  static const ci::Color kCoordinatesTextColor;
  static const ci::Font kCoordinatesTextFont;
  static const float kCoordinatesSpacing;
  static const float kFieldOfViewAngle;
  static const size_t kChunkRadius;
  static const size_t kRenderRadius;

 public:
  MinecraftApp();
  void draw() override;
  void update() override;
  void keyDown(ci::app::KeyEvent e) override;

 private:
  Camera camera_;
  World world_;
  std::vector<int> current_chunk_;

  void DrawUI();
  void MoveIfPossible(float delta_x, float delta_y);
  bool BlockExistsAt(float delta_x, float delta_y, float delta_z);
  void PanScreen(const ci::vec2& mouse_point);
  static bool IsBoundedBy(const ci::vec2& point, float x_min, float x_max,
                          float y_min, float y_max);
};

}  // namespace minecraft

#endif  // MINECRAFT_GAME_ENGINE_H
