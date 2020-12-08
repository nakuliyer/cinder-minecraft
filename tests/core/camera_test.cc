#include "core/camera.h"

#include <catch2/catch.hpp>

using ci::vec3;
using minecraft::Camera;

bool epsilonEqual(const vec3& first, const vec3& second) {
  return glm::length(first - second) < 0.01f;
}

TEST_CASE("Camera can move") {
  Camera camera(vec3(0, 0, 0));

  SECTION("+x direction") {
    camera.TransformX(0.4f);
    REQUIRE(camera.GetTransform() == vec3(0.4, 0, 0));
  }

  SECTION("-y direction") {
    camera.TransformY(-0.2f);
    REQUIRE(camera.GetTransform() == vec3(0, -0.2, 0));
  }

  SECTION("-z direction") {
    camera.TransformZ(-12.0f);
    REQUIRE(camera.GetTransform() == vec3(0, 0, -12.0));
  }
}

TEST_CASE("Camera can rotate and get a forward unit vector") {
  Camera camera(vec3(0, 0, 0));
  REQUIRE(camera.GetForwardVector() == vec3(1, 0, 0));

  SECTION("+xy plane") {
    camera.RotateXY(0.4f);
    REQUIRE(epsilonEqual(camera.GetForwardVector(), vec3(0.921, 0.389, 0)));
  }

  SECTION("-xz plane") {
    camera.RotateXZ(-1.2f);
    REQUIRE(epsilonEqual(camera.GetForwardVector(), vec3(0.362, 0, -0.932)));
  }
}