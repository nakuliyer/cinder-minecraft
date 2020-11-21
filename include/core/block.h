#ifndef MINECRAFT_BLOCK_H
#define MINECRAFT_BLOCK_H

#include <string>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace minecraft {

static const std::string kMartianDirtSide =
    "/Users/nakul/Documents/CLionProjects/Cinder/my-projects/"
    "final-project-nakuliyer/src/core/moon_texture.png";

class Block {
 public:
  Block(const ci::vec3& center);
  void SetUp();
  void Render() const;

  ci::vec3 GetCenter() const { return center_; };

 private:
  ci::gl::Texture2dRef texture_top_; // TODO: do this dynamically
  ci::gl::Texture2dRef texture_side_;
  ci::TriMesh mesh_;
  ci::TriMesh top_mesh_;
  ci::vec3 center_;
};

}  // namespace minecraft

#endif  // MINECRAFT_BLOCK_H