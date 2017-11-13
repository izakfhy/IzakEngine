#pragma once


#include <memory>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Resource.h"

namespace IZE
{
  class Texture : public IZE::Resource
  {
  public:
    Texture() {};
    ~Texture();

    bool Initialize(const std::string& path);
    void Bind(GLenum TextureUnit);

    bool         m_AllocatedMemoryInGPU = false;
    std::string  m_fileName = "./Resources/Textures/test.png";
    std::string  m_type = "";
    GLenum       m_Target = GL_TEXTURE_2D;
    GLuint       m_textureID = 0;
    int          m_ImageWidth, m_ImageHeight, m_NumComponents;
    int          m_Border = 0;
  };

  typedef std::shared_ptr<Texture> TexPtr;

}
