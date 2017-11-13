#include "Texture.h"
#include <iostream>
namespace IZE
{

#include "Texture.h"

#include "image_io.h"

  Texture::~Texture()
  {
    if (m_Target > 0)
    {
      glDeleteTextures(1, &m_textureID);
      m_Target = 0;
      m_AllocatedMemoryInGPU = false;
    }
  }

  bool Texture::Initialize(const std::string & path)
  {
    m_fileName = path;
    std::string TexName = std::string(path, path.find_last_of('/') + 1);
    printf("\tLoading Texture: %s\n", TexName.c_str());

    unsigned char* Pixels = nullptr;
    if (!ReadImageFile(m_fileName.c_str(),
      &Pixels,
      &m_ImageWidth,
      &m_ImageHeight,
      &m_NumComponents))
    {
      if (!ReadImageFile("./Resources/Textures/test.png",
        &Pixels,
        &m_ImageWidth,
        &m_ImageHeight,
        &m_NumComponents))
      {
        std::cout << "Checkered Texture missing!" << std::endl;
        //ERROR_MSG("Checkered Texture missing!");
      }
    }

    if (!m_AllocatedMemoryInGPU)
    {
      glGenTextures(1, &m_textureID);
      std::cout << "Texture ID: "<<m_textureID << std::endl;
      /*  Bind corresponding texture ID */
      glBindTexture(m_Target, m_textureID);

      GLint internalFormat = m_NumComponents == 3 ? GL_SRGB : GL_SRGB_ALPHA;
      GLint format = m_NumComponents == 3 ? GL_RGB : GL_RGBA;

      if (path.find("NORM") != std::string::npos ||
        path.find("NRM") != std::string::npos ||
        path.find("EMASK") != std::string::npos)
        internalFormat = m_NumComponents == 3 ? GL_RGB : GL_RGBA;

      /*  Copy image data to graphics memory */
      glTexImage2D(m_Target,
        0,
        internalFormat,
        m_ImageWidth,
        m_ImageHeight,
        m_Border,
        format,
        GL_UNSIGNED_BYTE,
        Pixels);

      /*  Generate texture mipmaps. */
      glGenerateMipmap(m_Target);

      /* Set up texture behaviors */
      glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameterf(m_Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(m_Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // anisotropic filtering to the max!
      float aniso;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

      glBindTexture(m_Target, 0);

      /*  Done with raw image data so delete it */
      free(Pixels);

      m_AllocatedMemoryInGPU = true;
    }

    return true;
  }

  void Texture::Bind(GLenum TextureUnit)
  {
    glActiveTexture(TextureUnit);
    glBindTexture(m_Target, m_textureID);
  }

}