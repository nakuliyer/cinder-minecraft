#ifndef IDEAL_GAS_NAKULIYER_IDEAL_GAS_APP_H
#define IDEAL_GAS_NAKULIYER_IDEAL_GAS_APP_H

#include <string>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "visualizer/direction.h"

namespace minecraft {
namespace visualizer {

static const float kWindowSize = 575.0f;
static const float kCentralPartition = 0.5f;

static const float kMovementDistance = 0.1f;
static const float kRotationSpeed = 0.01f;  // in radians

class MinecraftApp : public ci::app::App {
 public:
  MinecraftApp();
  void draw() override;
  void update() override;
  void keyDown(ci::app::KeyEvent e) override;
  void mouseMove(ci::app::MouseEvent e) override;

 private:
  ci::vec3 camera_transform_;
  ci::vec2 camera_rotation_;

  void RotateCameraInDirection(Direction direction);

  static Direction MapMousePointToWorld(const ci::vec2& mouse_point);
  static ci::vec3 ProjectToUnitSphere(const ci::vec2& rotation);
  static bool IsBoundedBy(const ci::vec2& point, float x_min, float x_max,
                          float y_min, float y_max);
};

}  // namespace visualizer
}  // namespace minecraft

#endif  // IDEAL_GAS_NAKULIYER_IDEAL_GAS_APP_H
