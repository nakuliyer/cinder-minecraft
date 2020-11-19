#ifndef MINECRAFT_WORLD_MAP_H
#define MINECRAFT_WORLD_MAP_H

#include <vector>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace minecraft {

static const float kBlockWidth = 1;
static const float kPlayerHeight = 2;
static const float kLandingRoom = 2;

class WorldMap {
 public:
  WorldMap();
  void Render();
  bool isOnLand(const ci::vec3& transform);

 private:
  std::vector<ci::vec3> blocks_;
};

}  // namespace minecraft

#endif  // MINE_CRAFTY_WORLD_MAP_H
