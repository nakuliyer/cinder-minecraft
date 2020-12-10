#ifndef MINECRAFT_GAME_ENGINE_H
#define MINECRAFT_GAME_ENGINE_H

#include <cinder/Camera.h>
#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>

#include <map>
#include <string>
#include <vector>

#include "core/camera.h"
#include "core/terrain_generator.h"
#include "core/world.h"

namespace minecraft {

/// the main cinder app
class MinecraftApp : public ci::app::App {
  /// window size
  static const float kWindowSize;
  /// the percent of the screen's width/height in the middle where mouse
  /// position does not pan the camera
  static const float kCentralPartition;
  /// distance to move when any move key is pressed
  static const float kMoveDistance;
  /// acceleration upwards on jump
  static const float kJumpForce;
  /// acceleration downwards
  static const float kGravityForce;
  /// rotation speed when mouse pans
  static const float kRotationSpeed;
  /// the player's height, used to break fall on the group
  static const float kPlayerHeight;
  /// attributes for the UI text
  static const ci::vec2 kLeftUITextPosition;
  /// attributes for the UI text
  static const ci::vec2 kRightUITextPosition;
  /// attributes for the UI text
  static const ci::Color kUITextColor;
  /// attributes for the UI text
  static const ci::Font kUITextFont;
  /// attributes for the UI text
  static const float kUITextSpacing;
  /// attributes for the icons in the UI
  static const float kUIIconSpacing;
  /// star next to the selected icon
  static const char kUIIconSelectedMarker;
  /// icon size in UI
  static const ci::vec2 kUIIconSize;
  /// field of view angle, blocks are not rendered outside this
  static const float kFieldOfViewAngle;
  /// radius of the chunks, see `world.h` for the usage
  static const size_t kChunkRadius;
  /// maximum distance from player to render blocks in
  static const size_t kRenderRadius;
  /// starting position
  static const ci::vec3 kPlayerStartingPosition;
  /// minimum height of terrain, i.e. sea level
  static const int kMinTerrainHeight;
  /// maximum height of terrain
  static const int kMaxTerrainHeight;
  /// variance of terrain, see `terrain_generator.h` for usage
  static const float kTerrainVariance;
  /// maximum seed length
  static const size_t kMaxSeedLength;
  /// angle difference (in radians) allowed between displacement and the
  /// camera's forward vector for a block to be considered "in the direction" of
  /// the camera
  static const float kDirectionalAngleAllowance;
  /// all blocks
  static const std::vector<BlockTypes> kOrderedBlocks;

 public:
  /// creates a minecraft app
  MinecraftApp();

  /// draws the UI to the screen, renders the camera, and renders all blocks.
  /// see `camera.h` and `world.h`
  void draw() override;

  /// checks where the mouse is to pan the camera, applies gravity, and checks
  /// whether the player has moved between chunks for chunk loading/deleting
  /// (see `world.h`)
  void update() override;

  /// applies key-down methods
  void keyDown(ci::app::KeyEvent e) override;

 private:
  /// world seed
  int seed_;
  /// camera
  Camera camera_;
  /// terrain generator (using Perlin noise)
  TerrainGenerator terrain_generator_;
  /// world, chunk handler
  World world_;
  /// current chunk
  std::vector<int> current_chunk_;
  /// player's current inventory
  std::map<BlockTypes, size_t> inventory_;
  /// the index of the block in kOrderedBlocks that the player is currently
  /// trying to place
  int current_placing_type_;

  /// draws coordinates at the top of the screen
  void DrawCoordinatesInterface();

  /// draws icons at the bottom
  void DrawIconsInterface();

  /// moves in the x or z direction if there is no obstacle blocking the move
  ///
  /// \param delta_x x distance
  /// \param delta_z z distance
  void MoveIfPossible(float delta_x, float delta_z);

  /// applies gravity to the camera if the player is not on ground
  void ApplyGravityIfNecessary();

  /// derived through projectile motion physics
  void JumpIfPossible();

  /// deletes the block that the player is looking at if there is such a block
  void DeleteBlockIfPossible();

  /// creates a block if there is a current hit box (next to the block the
  /// player is currently looking at) and we have enough of the requested type
  /// of block
  void CreateBlockIfPossible();

  /// increments/decrements `current_placing_type_` and mods around the list
  ///
  /// \param direction +1 for incrementing, -1 for decrementing
  void SwitchCurrentPlacingType(int direction);

  /// \param delta_x distance from camera
  /// \param delta_y distance from camera
  /// \param delta_z distance from camera
  /// \return whether there is a block there
  bool BlockExistsAt(float delta_x, float delta_y, float delta_z);

  /// rotates camera if the mouse is in the corresponding region of the screen
  ///
  /// \param mouse_point mouse location
  void PanScreen(const ci::vec2& mouse_point);

  /// helper for `PanScreen`
  ///
  /// \param point mouse location
  /// \param x_min region boundary
  /// \param x_max region boundary
  /// \param y_min region boundary
  /// \param y_max region boundary
  /// \return true if and only if the mouse location is inside the region
  static bool IsBoundedBy(const ci::vec2& point, float x_min, float x_max,
                          float y_min, float y_max);
};

}  // namespace minecraft

#endif  // MINECRAFT_GAME_ENGINE_H
