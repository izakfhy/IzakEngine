#pragma once
#include "BBEMath.h"
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"

struct Vertex {
  Vector3 Position;
  Vector3 Normal;
  Vector2 TexCoords;
};


class Mesh {
public:
  /*  Mesh Data  */
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<IZE::Texture>> textures;
  /*  Functions  */
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<IZE::Texture>> textures);
  void Draw(std::shared_ptr<Shader> shader);

  ~Mesh();
private:
  /*  Render data  */
  unsigned int VAO, VBO, EBO;
  /*  Functions    */
  void setupMesh();
};
