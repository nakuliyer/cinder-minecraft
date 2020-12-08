#include "core/camera.h"

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

Camera::Camera(const vec3 &initial_position, float terminal_velocity)
    : transform_(initial_position), terminal_velocity_(terminal_velocity) {
}

void Camera::Render() const {
  enableDepthRead();
  enableDepthWrite();

  CameraPersp cam;
  cam.lookAt(transform_, transform_ + GetForwardVector());
  setMatrices(cam);
}

vec3 Camera::GetForwardVector() const {
  // spherical coordinates implementation
  return vec3(cos(rotation_.y) * cos(rotation_.x), sin(rotation_.y),
              cos(rotation_.y) * sin(rotation_.x));
}

void Camera::ApplyYForce(float force) {
  y_velocity_ = std::max(y_velocity_ + force, terminal_velocity_);
  transform_.y += y_velocity_;
}

void Camera::ApplyNormalForce() {
  y_velocity_ = 0;
}

void Camera::TransformX(float distance) {
  transform_.x += distance;
}

void Camera::TransformY(float distance) {
  transform_.y += distance;
}

void Camera::TransformZ(float distance) {
  transform_.z += distance;
}

void Camera::RotateXZ(float radians) {
  rotation_.x += radians;
}

void Camera::RotateXY(float radians) {
  if (-M_PI / 2.0f < rotation_.y + radians &&
      rotation_.y + radians < M_PI / 2.0f) {
    rotation_.y += radians;
  }
}

}  // namespace minecraft