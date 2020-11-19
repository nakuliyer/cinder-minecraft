#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include <string>

#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

namespace minecraft {

class Camera {
  const float kTerminalVelocity = -6.0f;

 public:
  Camera() = default;
  void Render() const;
  ci::vec3 GetForwardVector() const;
  void ApplyYForce(float force);
  void ApplyNormalForce();

  ci::vec3 GetTransform() const { return transform_; };
  void TransformX(float distance);
  void TransformY(float distance);
  void TransformZ(float distance);
  void RotateXZ(float radians);
  void RotateXY(float radians);

 private:
  ci::vec3 transform_;
  ci::vec2 rotation_;
  float y_velocity_;
};

}  // namespace minecraft

#endif  // MINECRAFT_CAMERA_H
