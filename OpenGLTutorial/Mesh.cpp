#include "Mesh.h"
#include "ResourceManager.h"

Mesh::Mesh(std::vector<Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<std::shared_ptr<IZE::Texture>> textures_) :
vertices(vertices_),
indices(indices_),
textures(textures_)
{
  setupMesh();
}


Mesh::~Mesh()
{
  glDeleteBuffers(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(std::shared_ptr<Shader> shader)
{
  // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;
  for (unsigned int i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::stringstream ss;
    std::string number;
    std::string name = textures[i]->m_type;
    if (name == "diffuse")
      ss << diffuseNr++; // transfer unsigned int to stream
    else if (name == "specular")
      ss << specularNr++; // transfer unsigned int to stream
    else if (name == "normal")
      ss << normalNr++; // transfer unsigned int to stream
    else if (name == "height")
      ss << heightNr++; // transfer unsigned int to stream
    number = ss.str();
    // now set the sampler to the correct texture unit
    glUniform1i(glGetUniformLocation(shader->Program, ("material" + name).c_str()), i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures[i]->m_textureID);
  }

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}


void Mesh::setupMesh()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}