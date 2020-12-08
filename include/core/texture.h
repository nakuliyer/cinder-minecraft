#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H

#include <string>

#include "cinder/gl/gl.h"
#include "core/block_types.h"

namespace minecraft {

static const std::string kAssetsRoot = "../resources/";

static const std::map<BlockTypes, std::string> kTextureFiles = {
    {kGrass, "grass.png"}, {kDirt, "dirt.png"}};

class Texture {
 public:
  explicit Texture(const BlockTypes& block_type);
  ci::gl::Texture2dRef GetTexture();

 private:
  BlockTypes block_type_;

  std::string GetTextureFileName();
};

}  // namespace minecraft

#endif  // MINECRAFT_TEXTURE_H
