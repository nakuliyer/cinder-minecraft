#include "game_engine.h"

#include "cinder/app/Window.h"

using ci::CameraPersp;
using ci::vec2;
using ci::vec3;
using ci::app::KeyEvent;
using ci::app::MouseEvent;
using ci::app::setWindowSize;
using ci::app::Window;
using ci::gl::clear;
using ci::gl::drawCube;
using ci::gl::setMatrices;
using ci::gl::enableDepthRead;
using ci::gl::enableDepthWrite;
using minecraft::Camera;

namespace minecraft {

MinecraftApp::MinecraftApp() {
  setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void MinecraftApp::draw() {
  clear();
  camera_.Render();
  world_map_.Render();
}

void MinecraftApp::update() {
  vec2 mouse_point = getWindow()->getMousePos();
  if (!IsBoundedBy(mouse_point, 0, kWindowSize, 0, kWindowSize)) {
    return;
  }
  PanScreen(mouse_point);
}

void MinecraftApp::keyDown(KeyEvent e) {
  vec3 unit_sphere_directions = camera_.GetForwardVector();
  switch (e.getCode()) {
    case KeyEvent::KEY_w:
      camera_.TransformX(kMovementDistance * unit_sphere_directions.x);
      camera_.TransformZ(kMovementDistance * unit_sphere_directions.z);
      break;
    case KeyEvent::KEY_d:
      camera_.TransformX(-kMovementDistance * unit_sphere_directions.z);
      camera_.TransformZ(kMovementDistance * unit_sphere_directions.x);
      break;
    case KeyEvent::KEY_s:
      camera_.TransformX(-kMovementDistance * unit_sphere_directions.x);
      camera_.TransformZ(-kMovementDistance * unit_sphere_directions.z);
      break;
    case KeyEvent::KEY_a:
      camera_.TransformX(kMovementDistance * unit_sphere_directions.z);
      camera_.TransformZ(-kMovementDistance * unit_sphere_directions.x);
      break;
    case KeyEvent::KEY_SPACE:
      camera_.TransformY(kMovementDistance);
      break;
    case KeyEvent::KEY_LSHIFT:
      camera_.TransformY(-kMovementDistance);
      break;
    default:
      break;
  }
}

void MinecraftApp::mouseMove(MouseEvent e) {
  // TODO: this is where clicks are handled
}

void MinecraftApp::PanScreen(const ci::vec2& mouse_point) {
  float center_min = 0.5f * kWindowSize * (1 - kCentralPartition);
  float center_max = 0.5f * kWindowSize * (1 + kCentralPartition);
  if (IsBoundedBy(mouse_point, 0, center_min, 0, kWindowSize)) {
    camera_.RotateXZ(-kRotationSpeed);
  } else if (IsBoundedBy(mouse_point, center_max, kWindowSize, 0,
                         kWindowSize)) {
    camera_.RotateXZ(kRotationSpeed);
  } else if (IsBoundedBy(mouse_point, center_min, center_max, 0, center_min)) {
    camera_.RotateXY(kRotationSpeed);
  } else if (IsBoundedBy(mouse_point, center_min, center_max, center_max,
                         kWindowSize)) {
    camera_.RotateXY(-kRotationSpeed);
  }
}

bool MinecraftApp::IsBoundedBy(const vec2& point, float x_min, float x_max,
                               float y_min, float y_max) {
  // using the cs coordinate system (top left is 0, 0)
  return point.x >= x_min && point.x <= x_max && point.y >= y_min &&
         point.y <= y_max;
}

}  // namespace minecraft