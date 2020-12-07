#ifndef MINECRAFT_MAP_H
#define MINECRAFT_MAP_H

#include <map>
#include "cinder/gl/gl.h"
#include "core/block_types.h"

namespace minecraft {

class Map {
 public:

 private:
  std::map<ci::vec3, BlockTypes> map;
};

}

#endif  // MINECRAFT_MAP_H
