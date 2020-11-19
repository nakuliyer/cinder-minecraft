#ifndef MINECRAFT_WORLD_MAP_H
#define MINECRAFT_WORLD_MAP_H

#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/block.h"

namespace minecraft {
static const float kPlayerHeight = 2;
static const float kLandingRoom = 2;

class WorldMap {
 public:
  WorldMap();
  void Render(const ci::vec3& camera_transform, const ci::vec3& camera_forward);
  bool isOnLand(const ci::vec3& transform);

 private:
  std::vector<Block> blocks_;
};

}  // namespace minecraft

#endif  // MINE_CRAFTY_WORLD_MAP_H
