#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <string>
#include <vector>

#include "cinder/gl/gl.h"
#include "core/block_types.h"

namespace minecraft {

static const std::string kMartianDirtSide =
    "/Users/nakul/Documents/CLionProjects/Cinder/my-projects/"
    "final-project-nakuliyer/src/core/moon_texture.png";

class Block {
 public:
  Block(const BlockTypes& b, const ci::vec3& center);
  void SetUp();
  void Render() const;

  ci::vec3 GetCenter() const {
    return center_;
  };

 private:
  ci::gl::Texture2dRef texture_;
  ci::TriMesh mesh_;
  ci::vec3 center_;
  BlockTypes block_type_;
};

}  // namespace minecraft

#endif  // MINECRAFT_BLOCK_H