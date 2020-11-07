#ifndef IDEAL_GAS_NAKULIYER_IDEAL_GAS_APP_H
#define IDEAL_GAS_NAKULIYER_IDEAL_GAS_APP_H

#include <string>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace minecraft {
namespace visualizer {

class MinecraftApp : public ci::app::App {
  const float kWindowSize = 575;

 public:
  MinecraftApp();
  void draw() override;
  void update() override;
  void keyDown(ci::app::KeyEvent e) override;

 private:
  ci::vec3 camera_transform_;
};

}  // namespace visualizer
}  // namespace minecraft

#endif  // IDEAL_GAS_NAKULIYER_IDEAL_GAS_APP_H
