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
using ci::gl::enableDepthRead;
using ci::gl::enableDepthWrite;
using ci::gl::setMatrices;
using minecraft::Camera;

namespace minecraft {

MinecraftApp::MinecraftApp() {
  setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void MinecraftApp::draw() {
  clear();
  camera_.Render();
  world_map_.Render(camera_.GetTransform(), camera_.GetForwardVector());
}

void MinecraftApp::update() {
  vec2 mouse_point = getWindow()->getMousePos();
  if (IsBoundedBy(mouse_point, 0, kWindowSize, 0, kWindowSize)) {
    PanScreen(mouse_point);
  }
  if (BlockExistsAt(0, -kPlayerHeight, 0)) {
    camera_.ApplyNormalForce();
  } else {
    camera_.ApplyYForce(-kGravityForce);
  }
}

void MinecraftApp::keyDown(KeyEvent e) {
  vec3 forward = camera_.GetForwardVector();
  switch (e.getCode()) {
    case KeyEvent::KEY_w:
      MoveIfPossible(kMoveDistance * forward.x, kMoveDistance * forward.z);
      break;
    case KeyEvent::KEY_d:
      MoveIfPossible(-kMoveDistance * forward.z, kMoveDistance * forward.x);
      break;
    case KeyEvent::KEY_s:
      MoveIfPossible(-kMoveDistance * forward.x, -kMoveDistance * forward.z);
      break;
    case KeyEvent::KEY_a:
      MoveIfPossible(kMoveDistance * forward.z, -kMoveDistance * forward.x);
      break;
    case KeyEvent::KEY_SPACE:
      if (BlockExistsAt(0, -kPlayerHeight, 0)) {
        camera_.ApplyYForce(kJumpForce);
      }
      break;
    default:
      break;
  }
}

void MinecraftApp::mouseMove(MouseEvent e) {
  // TODO: this is where clicks are handled
}

void MinecraftApp::MoveIfPossible(float delta_x, float delta_z) {
  for (int delta_y = 0; delta_y < int(round(kPlayerHeight)); ++delta_y) {
    if (BlockExistsAt(delta_x, -float(delta_y), delta_z)) {
      return;
    }
  }
  camera_.TransformX(delta_x);
  camera_.TransformZ(delta_z);
}

bool MinecraftApp::BlockExistsAt(float delta_x, float delta_y, float delta_z) {
  vec3 location = camera_.GetTransform() + vec3(delta_x, delta_y, delta_z);
  return world_map_.GetBlockAt(location) != BlockTypes::kNone;
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