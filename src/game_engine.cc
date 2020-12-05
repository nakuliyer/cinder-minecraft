#include "game_engine.h"

#include "cinder/Utilities.h"
#include "cinder/app/Window.h"

using ci::CameraPersp;
using ci::Color;
using ci::Font;
using ci::vec2;
using ci::vec3;
using ci::app::KeyEvent;
using ci::app::MouseEvent;
using ci::app::setWindowSize;
using ci::app::Window;
using ci::gl::clear;
using ci::gl::drawCube;
using ci::gl::drawString;
using ci::gl::drawStrokedCube;
using ci::gl::enableDepthRead;
using ci::gl::enableDepthWrite;
using ci::gl::Fbo;
using ci::gl::FboRef;
using ci::gl::setMatrices;
using ci::gl::setMatricesWindow;
using minecraft::Camera;
using std::to_string;

namespace minecraft {

const vec2 MinecraftApp::kCoordinatesTextTopLeft = vec2(10, 10);
const Color MinecraftApp::kCoordinatesTextColor = Color(0, 255, 0);
const Font MinecraftApp::kCoordinatesTextFont = Font("Courier-Bold", 18.0f);
const float MinecraftApp::kCoordinatesSpacing = 20.0f;

MinecraftApp::MinecraftApp() {
  setWindowSize((int)kWindowSize, (int)kWindowSize);
}

void MinecraftApp::draw() {
  clear();
  setMatricesWindow(getWindowSize());
  DrawUI();
  camera_.Render();
  world_map_.Render(camera_.GetTransform(), camera_.GetForwardVector());
  drawStrokedCube(world_map_.GetClosestBlock(camera_.GetTransform(),
                                             camera_.GetForwardVector()),
                  vec3(1, 1, 1));
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
  if (world_map_.HasMovedChunks(camera_.GetTransform())) {
    world_map_.MoveToChunk(camera_.GetTransform());
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
    case KeyEvent::KEY_q:
      world_map_.DeleteClosestBlock(camera_.GetTransform(),
                                    camera_.GetForwardVector());
      break;
    default:
      break;
  }
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

void MinecraftApp::DrawUI() {
  drawString("x: " + to_string(int(camera_.GetTransform().x)),
             kCoordinatesTextTopLeft, kCoordinatesTextColor,
             kCoordinatesTextFont);
  drawString("y: " + to_string(int(camera_.GetTransform().y)),
             kCoordinatesTextTopLeft + vec2(0, kCoordinatesSpacing),
             kCoordinatesTextColor, kCoordinatesTextFont);
  drawString("z: " + to_string(int(camera_.GetTransform().z)),
             kCoordinatesTextTopLeft + vec2(0, 2 * kCoordinatesSpacing),
             kCoordinatesTextColor, kCoordinatesTextFont);
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