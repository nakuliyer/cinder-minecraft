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

namespace minecraft {
namespace visualizer {

MinecraftApp::MinecraftApp() {
  camera_transform_ = cinder::vec3(0, 0, 0);
  camera_focus_distance_ = cinder::vec3(1, 0, 0);
  setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void MinecraftApp::draw() {
  clear();

  CameraPersp cam;
  cam.lookAt(camera_transform_, camera_transform_ + camera_focus_distance_);
  setMatrices(cam);

  drawCube(vec3(4, 1, 0), vec3(1, 1, 1));
}

void MinecraftApp::update() {
}

void MinecraftApp::keyDown(KeyEvent e) {
  switch (e.getCode()) {
    case KeyEvent::KEY_w:
      camera_transform_.x += kMovementDistance;
      break;
    case KeyEvent::KEY_d:
      camera_transform_.z += kMovementDistance;
      break;
    case KeyEvent::KEY_s:
      camera_transform_.x -= kMovementDistance;
      break;
    case KeyEvent::KEY_a:
      camera_transform_.z -= kMovementDistance;
      break;
    default:
      break;
  }
}

void MinecraftApp::mouseMove(MouseEvent e) {
  // TODO: this is also where clicks are handled, which should be a sep method
  vec2 two_d_position = getWindow()->getMousePos();

  if (two_d_position.x < 0 || two_d_position.y < 0 ||
      two_d_position.x > kWindowSize || two_d_position.y > kWindowSize) {
    return;  // out of window bounds
  }

  vec3 projected = vec3(1, (kWindowSize - 2 * two_d_position.y) / kWindowSize,
                        (2 * two_d_position.x - kWindowSize) / kWindowSize);

  camera_focus_distance_ = projected;

  std::cout << projected << std::endl;
}

}  // namespace visualizer
}  // namespace minecraft