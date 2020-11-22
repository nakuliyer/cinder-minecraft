#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <string>
#include <vector>

#include "cinder/gl/gl.h"
#include "core/block_types.h"

namespace minecraft {

class Block {
  static const size_t kSquareVerticesCount = 4;
  static const size_t kCubeVerticesCount = 8;
  static const size_t kCubeFacesCount = 6;
  static const ci::vec3 kCubeVertices[kCubeVerticesCount];
  static const ci::vec3 kCubeFaces[kCubeFacesCount][kSquareVerticesCount];

 public:
  Block(const BlockTypes& b, const ci::vec3& center);
  void SetUp();
  void Render() const;
  BlockTypes GetType() const;
  ci::vec3 GetCenter() const;

 private:
  ci::gl::Texture2dRef texture_;
  ci::TriMesh mesh_;
  ci::vec3 center_;
  BlockTypes block_type_;
};

}  // namespace minecraft

#endif  // MINECRAFT_BLOCK_H