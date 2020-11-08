#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include <string>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace minecraft {

class Camera {
 public:
  Camera() = default;
  void Render() const;
  ci::vec3 GetForwardVector() const;

  void TransformX(float distance);
  void TransformY(float distance);
  void TransformZ(float distance);
  void RotateXZ(float radians);
  void RotateXY(float radians);

 private:
  ci::vec3 transform_;
  ci::vec2 rotation_;
};

}  // namespace minecraft

#endif  // MINECRAFT_CAMERA_H
