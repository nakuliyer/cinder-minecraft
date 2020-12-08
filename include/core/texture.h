#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H

#include <string>

#include "cinder/gl/gl.h"
#include "core/block_types.h"

namespace minecraft {

class Texture {
  static const std::map<BlockTypes, std::string> kTextureFiles;
  static const std::string kTestTexture;
 public:
  explicit Texture(const BlockTypes& block_type);
  ci::gl::Texture2dRef GetTexture();

 private:
  BlockTypes block_type_;

  std::string GetTextureFileName();
};

}  // namespace minecraft

#endif  // MINECRAFT_TEXTURE_H
