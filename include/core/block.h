#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <cinder/gl/gl.h>

#include <string>
#include <vector>

#include "block_types.h"

namespace minecraft {

/// cinder-compatible block
class Block {
  /// count for use in cube faces rendering
  static const size_t kSquareVerticesCount = 4;
  /// count for use in cube rendering
  static const size_t kCubeVerticesCount = 8;
  /// count for use in cube rendering
  static const size_t kCubeFacesCount = 6;
  /// the locations of vertices in a cube centered at the origin
  static const ci::vec3 kCubeVertices[kCubeVerticesCount];
  /// the faces of a cube with respect to the vertices in `kCubeVertices`
  static const ci::vec3 kCubeFaces[kCubeFacesCount][kSquareVerticesCount];

 public:
  /// creates a block
  ///
  /// \param block_type the type of block
  /// \param center the center of this block
  Block(const BlockTypes& block_type, const ci::vec3& center);
  /// renders this block
  void Render() const;
  /// \return type of this block
  BlockTypes GetType() const;
  /// \return center of this block
  ci::vec3 GetCenter() const;

 private:
  /// this block's type
  BlockTypes block_type_;
  /// center of this block
  ci::vec3 center_;
  /// texture of this block
  ci::gl::Texture2dRef texture_;
  /// mesh for this block
  ci::TriMesh mesh_;

  /// sets up mesh and texture for this block
  void SetUp();
};

}  // namespace minecraft

#endif  // MINECRAFT_BLOCK_H