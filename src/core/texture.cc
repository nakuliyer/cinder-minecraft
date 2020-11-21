#include "core/texture.h"

using std::invalid_argument;
using std::string;
using ci::gl::Texture2dRef;
using ci::gl::Texture2d;
using ci::loadImage;

namespace minecraft {

Texture::Texture(const BlockTypes &block_type) : block_type_(block_type) {
}

Texture2dRef Texture::GetTexture() {
  string file = kAssetsRoot + GetTextureFileName();
  return Texture2d::create(loadImage(file));
}

string Texture::GetTextureFileName() {
  if (kTextureFiles.find(block_type_) != kTextureFiles.end()) {
    return kTextureFiles.at(block_type_);
  }
  throw invalid_argument("Cannot get texture of null block.");
}

}  // namespace minecraft