#include "core/block.h"

using ci::Color;
using ci::loadImage;
using ci::TriMesh;
using ci::vec2;
using ci::vec3;
using ci::gl::Texture2d;
using ci::gl::Texture2dRef;
using ci::gl::VboMesh;
using std::vector;

namespace minecraft {

Block::Block(const vec3& center) {
  // https://mottosso.gitbooks.io/cinder/content/book/guide_to_meshes.html
  // https://drewish.com/2014/08/23/using-cinder%27s-cameraortho-and-vbomesh-to-draw-cubes/
  center_ = center;
}

void Block::SetUp() {
  texture_ = Texture2d::create(loadImage(kGrassTexture));
  mesh_ = TriMesh(TriMesh::Format().positions().texCoords(2));
  vec2 texture_vertices[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

  vec3 vertices[8] = {{-0.5, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
                      {0.5f, 0.5f, -0.5f},  {-0.5f, 0.5f, -0.5f},
                      {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
                      {0.5f, 0.5f, 0.5f},   {-0.5f, 0.5f, 0.5f}};
  for (vec3& vertex : vertices) {
    vertex += center_;
  }
  vec3 faces[6][4] = {{vertices[0], vertices[1], vertices[2], vertices[3]},
                      {vertices[3], vertices[2], vertices[6], vertices[7]},
                      {vertices[7], vertices[6], vertices[5], vertices[4]},
                      {vertices[4], vertices[5], vertices[1], vertices[0]},
                      {vertices[5], vertices[6], vertices[2], vertices[1]},
                      {vertices[7], vertices[4], vertices[0], vertices[3]}};
  //  vec3 faces[1][4] = {{vertices[4], vertices[5], vertices[1], vertices[0]}};


  for (auto& face : faces) {
    for (size_t vertex = 0; vertex < 4; vertex++) {
      mesh_.appendPosition(face[vertex]);
      mesh_.appendTexCoord(texture_vertices[vertex]);
    }
    size_t numberVertices = mesh_.getNumVertices();
    mesh_.appendTriangle(numberVertices - 4, numberVertices - 3,
                         numberVertices - 2);
    mesh_.appendTriangle(numberVertices - 4, numberVertices - 2,
                         numberVertices - 1);
  }
}

void Block::Render() const {
  ci::gl::ScopedGlslProg glslScope{
      ci::gl::getStockShader(ci::gl::ShaderDef().texture())};
  ci::gl::ScopedTextureBind texScope{texture_};

  ci::gl::draw(mesh_);
}

}  // namespace minecraft
