#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H

#include <string>

#include "cinder/gl/gl.h"
#include "block_types.h"

namespace minecraft {

static const std::string kAssetsRoot =
    "/Users/nakul/Documents/CLionProjects/Cinder/my-projects/"
    "final-project-nakuliyer/src/core/";

static const std::map<BlockTypes, std::string> kTextureFiles = {
    {kGrass, "moon_texture.png"}
};

class Texture {
 public:
  explicit Texture(const BlockTypes& block_type);
  ci::gl::Texture2dRef GetTexture();

 private:
  BlockTypes block_type_;

  std::string GetTextureFileName();
};

}  // namespace minecraft

// std::string block_to_texture(BlockTypes block_type) {
//  switch (block_type) {
//    case BlockTypes::kGrass:
//      return "/Users/nakul/Documents/CLionProjects/Cinder/my-projects/"
//             "final-project-nakuliyer/src/core/texture.png";
//    default:
//      return "";
//  }
//}

#endif  // MINECRAFT_TEXTURE_H
