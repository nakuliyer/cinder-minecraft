#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <string>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace minecraft {

static const std::string kGrassTexture =
    "/Users/nakul/Documents/CLionProjects/Cinder/my-projects/"
    "final-project-nakuliyer/src/core/texture.png";

class Block {
 public:
  Block(const ci::vec3& center);
  void SetUp();
  void Render() const;

  ci::vec3 GetCenter() const { return center_; };

 private:
  ci::gl::Texture2dRef texture_;
  ci::TriMesh mesh_;
  ci::vec3 center_;
};

}  // namespace minecraft

#endif  // MINECRAFT_BLOCK_H