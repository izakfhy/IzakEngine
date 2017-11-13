#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "BBEMath.h"
#include "GL/glew.h"
#include "Resource.h"

class Shader : public IZE::Resource
{
public:
  // The program ID
  GLuint Program;
  std::string m_name = "";
  Shader(){}
  // Constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath);
  // Use the program
  void Use();
  void SetMat4F(const std::string& id, const Matrix4x4& mat);
  void SetVec3(const std::string &name, const Vector3& value) const;
  void SetVec4(const std::string &name, const Vector4& value) const;
  void SetFloat(const std::string &name, float value) const;
  void SetInt(const std::string &name, int value) const;
  bool LoadAndCompile(const std::string& vertexPath, const std::string& fragmentPath);

  virtual bool Initialize(const std::string& path) override final;
};
