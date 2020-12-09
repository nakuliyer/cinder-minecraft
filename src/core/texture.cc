#include "core/texture.h"

#include "cinder/app/app.h"

using ci::loadImage;
using ci::app::getAssetPath;
using ci::gl::Texture2d;
using ci::gl::Texture2dRef;
using std::invalid_argument;
using std::map;
using std::string;

namespace minecraft {

const map<BlockTypes, string> Texture::kTextureFiles = {
    {kGrass, "grass.png"}, {kDirt, "dirt.png"}, {kStone, "stone.png"}};
const map<BlockTypes, string> Texture::kIconFiles = {
    {kGrass, "grass_icon.png"},
    {kDirt, "dirt_icon.png"},
    {kStone, "stone_icon.png"}};
const string Texture::kTestTexture = "test.png";

Texture::Texture(const BlockTypes &block_type) : block_type_(block_type) {
}

#ifdef DONT_USE_TEXTURES
Texture2dRef Texture::GetTexture() {
  std::move(block_type_);  // avoid unused variable error
  return nullptr;
}
#elif defined USE_TEST_TEXTURES
Texture2dRef Texture::GetTexture() {
  return Texture2d::create(loadImage(getAssetPath(kTestTexture)));
}
#else
Texture2dRef Texture::GetTexture() {
  if (kTextureFiles.find(block_type_) != kTextureFiles.end()) {
    return Texture2d::create(
        loadImage(getAssetPath(kTextureFiles.at(block_type_))));
  }
  throw invalid_argument(std::to_string(block_type_) +
                         " does not have a texture");
}
#endif

Texture2dRef Texture::GetIcon(BlockTypes block_type) {
  if (kIconFiles.find(block_type) != kIconFiles.end()) {
    return Texture2d::create(
        loadImage(getAssetPath(kIconFiles.at(block_type))));
  }
  throw invalid_argument(std::to_string(block_type) + " does not have an icon");
}

}  // namespace minecraft