#ifndef MINECRAFT_CAMERA_H
#define MINECRAFT_CAMERA_H

#include <cinder/gl/gl.h>

#include <string>

namespace minecraft {

/// the player
class Camera {
 public:
  /// sets up a camera
  ///
  /// \param initial_position initial position
  /// \param terminal_velocity minimum y velocity
  Camera(const ci::vec3& initial_position, float terminal_velocity = -6.0f);

  /// sets up the cinder matrices for this camera and faces towards the current
  /// forward vector
  void Render() const;

  /// \return a unit vector in the forwards direction
  ci::vec3 GetForwardVector() const;

  /// compounds the current y velocity with force and transforms y by the
  /// velocity. if y velocity becomes lower than the terminal velocity, only
  /// transforms y by terminal velocity.
  ///
  /// \param force some force in the +y direction
  void ApplyYForce(float force);

  /// sets y velocity to zero
  void ApplyNormalForce();

  /// moves camera in +x
  /// \param distance some distance
  void TransformX(float distance);

  /// moves camera in +y
  /// \param distance some distance
  void TransformY(float distance);

  /// moves camera in +z
  /// \param distance some distance
  void TransformZ(float distance);

  /// rotates camera left-to-right
  /// \param radians degree rightwards
  void RotateXZ(float radians);

  /// rotates camera down-to-up
  /// \param radians degree upwards
  void RotateXY(float radians);

  /// \return transform (location) of this camera
  ci::vec3 GetTransform() const;

 private:
  /// position of the camera
  ci::vec3 transform_;
  /// rotation of the camera, the x-axis being XZ rotation and y-axis being XY
  /// rotation
  ci::vec2 rotation_;
  /// current y velocity
  float y_velocity_;
  /// minimum y velocity
  float terminal_velocity_;
};

}  // namespace minecraft

#endif  // MINECRAFT_CAMERA_H
