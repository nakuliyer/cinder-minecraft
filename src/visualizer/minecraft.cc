#include "visualizer/minecraft.h"

using ci::CameraPersp;
using ci::vec3;
using ci::app::setWindowSize;
using ci::gl::clear;
using ci::gl::setMatrices;
using ci::gl::drawCube;
using ci::app::KeyEvent;

namespace minecraft {
namespace visualizer {

MinecraftApp::MinecraftApp() {
  camera_transform_ = cinder::vec3(0, 0, 0);
  setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void MinecraftApp::draw() {
  clear();

  CameraPersp cam;
  cam.lookAt(camera_transform_, camera_transform_ + vec3(3, 1, 0));
  setMatrices(cam);

  drawCube(vec3(4, 2, 0), vec3(1, 1, 1));
}

void MinecraftApp::update() {
}

void MinecraftApp::keyDown(KeyEvent e) {
  switch (e.getCode()) {
    case KeyEvent::KEY_w:
      camera_transform_.x += 1;
      break;
    case KeyEvent::KEY_d:
      camera_transform_.z += 1;
      break;
    case KeyEvent::KEY_s:
      camera_transform_.x -= 1;
      break;
    case KeyEvent::KEY_a:
      camera_transform_.z -= 1;
      break;
    default:
      break;
  }
}

}  // namespace visualizer
}  // namespace minecraft