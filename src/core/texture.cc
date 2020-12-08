#include "core/texture.h"

#include "cinder/app/app.h"

using ci::loadImage;
using ci::app::getAssetPath;
using ci::gl::Texture2d;
using ci::gl::Texture2dRef;
using std::invalid_argument;
using std::string;

namespace minecraft {

Texture::Texture(const BlockTypes &block_type) : block_type_(block_type) {
}

#ifdef DONT_USE_TEXTURES
Texture2dRef Texture::GetTexture() {
  return nullptr;
}
#else
Texture2dRef Texture::GetTexture() {
  return Texture2d::create(loadImage(getAssetPath(GetTextureFileName())));
}
#endif

string Texture::GetTextureFileName() {
  if (kTextureFiles.find(block_type_) != kTextureFiles.end()) {
    return kTextureFiles.at(block_type_);
  }
  throw invalid_argument("Cannot get texture of null block.");
}

}  // namespace minecraft