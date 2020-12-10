#include "game_engine.h"

#include "cinder/Utilities.h"
#include "cinder/app/Window.h"
#include "core/texture.h"

using ci::CameraPersp;
using ci::Color;
using ci::Font;
using ci::Rectf;
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
using minecraft::Texture;
using std::pair;
using std::string;
using std::to_string;
using std::vector;

namespace minecraft {

const float MinecraftApp::kWindowSize = 575.0f;
const float MinecraftApp::kCentralPartition = 0.5f;
const float MinecraftApp::kMoveDistance = 0.8f;
const float MinecraftApp::kJumpForce = 0.3f;
const float MinecraftApp::kGravityForce = 0.03f;
const float MinecraftApp::kRotationSpeed = 0.05f;
const float MinecraftApp::kPlayerHeight = 2.0f;
const vec2 MinecraftApp::kLeftUITextPosition = vec2(10, 10);
const vec2 MinecraftApp::kRightUITextPosition = vec2(30, kWindowSize - 100);
const Color MinecraftApp::kUITextColor = Color(0, 255, 0);
const Font MinecraftApp::kUITextFont = Font("Courier-Bold", 18.0f);
const char MinecraftApp::kUIIconSelectedMarker = '*';
const float MinecraftApp::kUITextSpacing = 20.0f;
const float MinecraftApp::kUIIconSpacing = 25.0f;
const vec2 MinecraftApp::kUIIconSize = vec2(20, 20);
const float MinecraftApp::kFieldOfViewAngle = 1.0472f;
const size_t MinecraftApp::kChunkRadius = 2;   // increasing this significantly
                                               // impacts lag
const size_t MinecraftApp::kRenderRadius = 8;  // increasing this significantly
                                               // impacts lag, especially
                                               // underground
const vec3 MinecraftApp::kPlayerStartingPosition = vec3(0, 10, 0);
const int MinecraftApp::kMinTerrainHeight = -3;
const int MinecraftApp::kMaxTerrainHeight = 2;
const float MinecraftApp::kTerrainVariance = 10.0f;
const size_t MinecraftApp::kMaxSeedLength = 100000;
const float MinecraftApp::kDirectionalAngleAllowance = 0.3f;
const vector<BlockTypes> MinecraftApp::kOrderedBlocks = {
    BlockTypes::kGrass, BlockTypes::kDirt, BlockTypes::kStone};

MinecraftApp::MinecraftApp()
    : seed_(rand() % kMaxSeedLength),
      camera_(kPlayerStartingPosition),
      terrain_generator_(kMinTerrainHeight, kMaxTerrainHeight, kTerrainVariance,
                         seed_),
      world_(&terrain_generator_, kPlayerStartingPosition, kChunkRadius) {
  setWindowSize((int)kWindowSize, (int)kWindowSize);
  current_chunk_ = world_.GetChunk(kPlayerStartingPosition);
  for (const BlockTypes& block_type : kOrderedBlocks) {
    inventory_.insert(pair<BlockTypes, size_t>(block_type, 0));
  }
}

void MinecraftApp::draw() {
  clear();
  setMatricesWindow(getWindowSize());
  DrawCoordinatesInterface();
  DrawIconsInterface();
  camera_.Render();
  world_.Render(camera_.GetTransform(), camera_.GetForwardVector(),
                kFieldOfViewAngle, kRenderRadius);
  world_.OutlineBlockInDirectionOf(camera_.GetTransform(),
                                   camera_.GetForwardVector(),
                                   kDirectionalAngleAllowance);
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
  if (world_.HasMovedChunks(current_chunk_, camera_.GetTransform())) {
    vector<int> new_chunk = world_.GetChunk(camera_.GetTransform());
    world_.MoveToChunk(current_chunk_, new_chunk);
    current_chunk_ = new_chunk;
  }
}

void MinecraftApp::keyDown(KeyEvent e) {
  vec3 forward = camera_.GetForwardVector();
  if (e.getCode() == KeyEvent::KEY_w) {
    MoveIfPossible(kMoveDistance * forward.x, kMoveDistance * forward.z);
  } else if (e.getCode() == KeyEvent::KEY_d) {
    MoveIfPossible(-kMoveDistance * forward.z, kMoveDistance * forward.x);
  } else if (e.getCode() == KeyEvent::KEY_s) {
    MoveIfPossible(-kMoveDistance * forward.x, -kMoveDistance * forward.z);
  } else if (e.getCode() == KeyEvent::KEY_a) {
    MoveIfPossible(kMoveDistance * forward.z, -kMoveDistance * forward.x);
  } else if (e.getCode() == KeyEvent::KEY_SPACE) {
    JumpIfPossible();
  } else if (e.getCode() == KeyEvent::KEY_q) {
    DeleteBlockIfPossible();
  } else if (e.getCode() == KeyEvent::KEY_e) {
    CreateBlockIfPossible();
  } else if (e.getCode() == KeyEvent::KEY_DOWN) {
    SwitchCurrentPlacingType(1);
  } else if (e.getCode() == KeyEvent::KEY_UP) {
    SwitchCurrentPlacingType(-1);
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

void MinecraftApp::JumpIfPossible() {
  if (BlockExistsAt(0, -kPlayerHeight, 0)) {
    int max_height = ceil(kJumpForce * kJumpForce / (2 * kGravityForce));
    for (size_t i = 1; i <= max_height; ++i) {
      if (BlockExistsAt(0, i, 0)) {
        return;
      }
    }
    camera_.ApplyYForce(kJumpForce);
  }
}

void MinecraftApp::DeleteBlockIfPossible() {
  BlockTypes deleted = world_.DeleteBlockInDirectionOf(
      camera_.GetTransform(), camera_.GetForwardVector(),
      kDirectionalAngleAllowance);
  ++inventory_.at(deleted);
}

void MinecraftApp::CreateBlockIfPossible() {
  BlockTypes block_type(kOrderedBlocks.at(current_placing_type_));
  if (inventory_.at(block_type) > 0) {
    bool created = world_.CreateBlockInDirectionOf(
        camera_.GetTransform(), camera_.GetForwardVector(), block_type,
        kDirectionalAngleAllowance);
    if (created) {
      --inventory_.at(block_type);
    }
  }
}

void MinecraftApp::SwitchCurrentPlacingType(int direction) {
  current_placing_type_ += direction;
  if (current_placing_type_ == kOrderedBlocks.size()) {
    current_placing_type_ = 0;
  } else if (current_placing_type_ == -1) {
    current_placing_type_ = kOrderedBlocks.size() - 1;
  }
}

bool MinecraftApp::BlockExistsAt(float delta_x, float delta_y, float delta_z) {
  vec3 location = camera_.GetTransform() + vec3(delta_x, delta_y, delta_z);
  return world_.GetBlockAt(location) != BlockTypes::kNone;
}

void MinecraftApp::DrawCoordinatesInterface() {
  drawString("seed: " + to_string(seed_), kLeftUITextPosition, kUITextColor,
             kUITextFont);
  drawString("x: " + to_string(int(camera_.GetTransform().x)),
             kLeftUITextPosition + vec2(0, kUITextSpacing), kUITextColor,
             kUITextFont);
  drawString("y: " + to_string(int(camera_.GetTransform().y)),
             kLeftUITextPosition + vec2(0, 2 * kUITextSpacing), kUITextColor,
             kUITextFont);
  drawString("z: " + to_string(int(camera_.GetTransform().z)),
             kLeftUITextPosition + vec2(0, 3 * kUITextSpacing), kUITextColor,
             kUITextFont);
}

void MinecraftApp::DrawIconsInterface() {
  for (int i = 0; i < kOrderedBlocks.size(); ++i) {
    vec2 space = vec2(0, float(i) * kUIIconSpacing);
    Rectf icon(kRightUITextPosition + space,
               kRightUITextPosition + kUIIconSize + space);
    ci::gl::draw(Texture::GetIcon(kOrderedBlocks.at(i)), icon);
    char starred = current_placing_type_ == i ? kUIIconSelectedMarker : ' ';
    if (current_placing_type_ == i) {
      vec2 star_offset(-kUITextFont.getSize() / 2, kUIIconSize.y / 4);
      drawString(string(1, starred), kRightUITextPosition + space + star_offset,
                 kUITextColor, kUITextFont);
    }
    vec2 text_offset(kUIIconSize.x, kUIIconSize.y / 4);
    drawString(": " + to_string(inventory_.at(kOrderedBlocks.at(i))),
               kRightUITextPosition + space + text_offset, kUITextColor,
               kUITextFont);
  }
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