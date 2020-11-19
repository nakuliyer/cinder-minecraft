#ifndef MINECRAFT_GAME_ENGINE_H
#define MINECRAFT_GAME_ENGINE_H

#include <string>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/camera.h"
#include "core/world_map.h"

namespace minecraft {

static const float kWindowSize = 575.0f;
static const float kCentralPartition = 0.5f;

static const float kMovementDistance = 0.5f;
static const float kJumpForce = 0.5f;       // velocity upwards on jump
static const float kGravityForce = 0.01f;   // acceleration downwards
static const float kRotationSpeed = 0.05f;  // in radians

class MinecraftApp : public ci::app::App {
 public:
  MinecraftApp();
  void draw() override;
  void update() override;
  void keyDown(ci::app::KeyEvent e) override;
  void mouseMove(ci::app::MouseEvent e) override;

 private:
  Camera camera_;
  WorldMap world_map_;

  void PanScreen(const ci::vec2& mouse_point);

  static bool IsBoundedBy(const ci::vec2& point, float x_min, float x_max,
                          float y_min, float y_max);
};

}  // namespace minecraft

#endif  // MINECRAFT_GAME_ENGINE_H
