#include "core/world_map.h"

using ci::vec3;
using ci::gl::drawStrokedCube;

namespace minecraft {

void WorldMap::Render() {
  for (int x = -4; x <= 4; ++x) {
    for (int z = -4; z <= 4; ++z) {
      drawStrokedCube(vec3(x, -2, z), vec3(1, 1, 1));
    }
  }
}

}