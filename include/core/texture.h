#ifndef MINECRAFT_TEXTURE_H
#define MINECRAFT_TEXTURE_H

#include <cinder/gl/gl.h>

#include <string>

#include "block_types.h"

namespace minecraft {

class Texture {
  /// mapping of block types to textures
  static const std::map<BlockTypes, std::string> kTextureFiles;
  /// a texture used for testing, where the faces of the block will be the
  /// numbers 1 to 6 in the face-order TOP, FRONT, RIGHT, BACK, LEFT, BOTTOM
  static const std::string kTestTexture;

 public:
  /// creates a texture
  ///
  /// \param block_type the block type for this texture (cannot be
  /// `BlockTypes::kNone`!)
  Texture(const BlockTypes& block_type);

  /// \return the texture
  ci::gl::Texture2dRef GetTexture();

 private:
  /// this block type
  BlockTypes block_type_;
};

}  // namespace minecraft

#endif  // MINECRAFT_TEXTURE_H
