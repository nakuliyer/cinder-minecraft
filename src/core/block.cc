#include "core/block.h"

#include "core/texture.h"

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

const vec3 Block::kCubeVertices[8] = {
    {-0.5, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
    {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
    {0.5f, 0.5f, 0.5f},   {-0.5f, 0.5f, 0.5f}};

// this ordering is very particular and determined through manual testing in the
// `minecraft-manual-test` app. the ordering corresponds to the particular
// vertices in `kCubeVertices`.
const vec3 Block::kCubeFaces[6][4] = {
    {kCubeVertices[3], kCubeVertices[7], kCubeVertices[6], kCubeVertices[2]},
    {kCubeVertices[0], kCubeVertices[4], kCubeVertices[7], kCubeVertices[3]},
    {kCubeVertices[4], kCubeVertices[5], kCubeVertices[6], kCubeVertices[7]},
    {kCubeVertices[5], kCubeVertices[1], kCubeVertices[2], kCubeVertices[6]},
    {kCubeVertices[1], kCubeVertices[0], kCubeVertices[3], kCubeVertices[2]},
    {kCubeVertices[1], kCubeVertices[5], kCubeVertices[4], kCubeVertices[0]}};

Block::Block(const BlockTypes& block_type, const vec3& center) {
  texture_ = Texture(block_type).GetTexture();
  block_type_ = block_type;
  center_ = center;
  SetUp();
}

void Block::Render() const {
  ci::gl::ScopedGlslProg glslScope{
      ci::gl::getStockShader(ci::gl::ShaderDef().texture())};
  ci::gl::ScopedTextureBind texScope{texture_};
  ci::gl::draw(mesh_);
}

BlockTypes Block::GetType() const {
  return block_type_;
}

vec3 Block::GetCenter() const {
  return center_;
}

void Block::SetUp() {
  // https://mottosso.gitbooks.io/cinder/content/book/guide_to_meshes.htmlss
  mesh_ = TriMesh(TriMesh::Format().positions().texCoords(2));
  vec2 texture_vertices[kSquareVerticesCount] = {
      {0, 0}, {1.0f / kCubeFacesCount, 0}, {1.0f / kCubeFacesCount, 1}, {0, 1}};
  for (size_t face = 0; face < kCubeFacesCount; ++face) {
    for (size_t vertex = 0; vertex < kSquareVerticesCount; ++vertex) {
      mesh_.appendPosition(kCubeFaces[face][vertex] + center_);
      vec2 face_texture_delta(face * 1.0f / kCubeFacesCount, 0);
      mesh_.appendTexCoord(texture_vertices[vertex] + face_texture_delta);
    }
    size_t vertices_count = mesh_.getNumVertices();
    mesh_.appendTriangle(vertices_count - 4, vertices_count - 3,
                         vertices_count - 2);
    mesh_.appendTriangle(vertices_count - 4, vertices_count - 2,
                         vertices_count - 1);
  }
}

}  // namespace minecraft
