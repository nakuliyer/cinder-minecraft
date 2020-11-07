// TODO: rename to camera
#include "visualizer/minecraft.h"

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

namespace minecraft {
namespace visualizer {

MinecraftApp::MinecraftApp() {
//  camera_transform_ = vec3(4, 0, -4);
//  camera_rotation_ = vec2(0, 3.14 / 2);
  camera_transform_ = vec3(0, 0, 0);
  camera_rotation_ = vec2(0, 0);
  setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void MinecraftApp::draw() {
  clear();
  enableDepthRead();
  enableDepthWrite();

  CameraPersp cam;
  vec3 camera_focus_distance = ProjectToUnitSphere(camera_rotation_);
  cam.lookAt(camera_transform_, camera_transform_ + camera_focus_distance);
  std::cout << camera_transform_ << " with rotation " << camera_rotation_ << " observing " << camera_focus_distance << std::endl;
  setMatrices(cam);

  drawCube(vec3(4, 0, 0), vec3(1, 1, 1));
}

void MinecraftApp::update() {
  vec2 mouse_point = getWindow()->getMousePos();

  if (!IsBoundedBy(mouse_point, 0, kWindowSize, 0, kWindowSize)) {
    return;
  }

  Direction mouse_direction = MapMousePointToWorld(mouse_point);
  RotateCameraInDirection(mouse_direction);
}

void MinecraftApp::keyDown(KeyEvent e) {
  vec3 unit_sphere_directions = ProjectToUnitSphere(camera_rotation_);
  switch (e.getCode()) {
    case KeyEvent::KEY_w:
      camera_transform_.x += kMovementDistance * unit_sphere_directions.x;
      camera_transform_.z += kMovementDistance * unit_sphere_directions.z;
      break;
    case KeyEvent::KEY_d:
      camera_transform_.x -= kMovementDistance * unit_sphere_directions.z;
      camera_transform_.z += kMovementDistance * unit_sphere_directions.x;
      break;
    case KeyEvent::KEY_s:
      camera_transform_.x -= kMovementDistance * unit_sphere_directions.x;
      camera_transform_.z -= kMovementDistance * unit_sphere_directions.z;
      break;
    case KeyEvent::KEY_a:
      camera_transform_.x += kMovementDistance * unit_sphere_directions.z;
      camera_transform_.z -= kMovementDistance * unit_sphere_directions.x;
      break;
    default:
      break;
  }
}

void MinecraftApp::mouseMove(MouseEvent e) {
  // TODO: this is where clicks are handled
  // camera_focus_distance_ = projected;

  // std::cout << projected << std::endl;
}

void MinecraftApp::RotateCameraInDirection(Direction direction) {
  switch (direction) {
    case Direction::kTop:
      camera_rotation_.y += kRotationSpeed;
      break;
    case Direction::kRight:
      camera_rotation_.x += kRotationSpeed;
      break;
    case Direction::kBottom:
      camera_rotation_.y -= kRotationSpeed;
      break;
    case Direction::kLeft:
      camera_rotation_.x -= kRotationSpeed;
      break;
    case Direction::kCenter:
      break;
  }
}

Direction MinecraftApp::MapMousePointToWorld(const vec2& mouse_point) {
  float center_min = 0.5f * kWindowSize * (1 - kCentralPartition);
  float center_max = 0.5f * kWindowSize * (1 + kCentralPartition);
  if (IsBoundedBy(mouse_point, 0, center_min, 0, kWindowSize)) {
    return Direction::kLeft;
  } else if (IsBoundedBy(mouse_point, center_max, kWindowSize, 0,
                         kWindowSize)) {
    return Direction::kRight;
  } else if (IsBoundedBy(mouse_point, center_min, center_max, 0, center_min)) {
    return Direction::kTop;
  } else if (IsBoundedBy(mouse_point, center_min, center_max, center_max,
                         kWindowSize)) {
    return Direction::kBottom;
  }
  return Direction::kCenter;
}

vec3 MinecraftApp::ProjectToUnitSphere(const vec2& rotation) {
  // spherical coordinates implementation
  return vec3(cos(rotation.y) * cos(rotation.x),
              sin(rotation.y), cos(rotation.y) * sin(rotation.x));
}

bool MinecraftApp::IsBoundedBy(const vec2& point, float x_min, float x_max,
                               float y_min, float y_max) {
  // using the cs coordinate system (top left is 0, 0)
  return point.x >= x_min && point.x <= x_max && point.y >= y_min &&
         point.y <= y_max;
}

}  // namespace visualizer
}  // namespace minecraft