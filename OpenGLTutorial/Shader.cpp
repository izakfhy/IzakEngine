#include "Shader.h"
#include <fstream>


bool Shader::Initialize(const std::string& path)
{
  return LoadAndCompile(path + ".vert", path + ".frag");

}

bool Shader::LoadAndCompile(const std::string& vertexPath, const std::string& fragmentPath)
{
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // ensures ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::badbit);
  try
  {
    // Open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // Convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    return false;
  }
  const GLchar* vShaderCode = vertexCode.c_str();
  const GLchar * fShaderCode = fragmentCode.c_str();
  // 2. Compile shaders
  GLuint vertex, fragment;
  GLint success;
  GLchar infoLog[512];
  // Vertex Shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  // Print compile errors if any
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    return false;
  }
  // Fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  // Print compile errors if any
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    return false;
  }
  // Shader Program
  this->Program = glCreateProgram();
  glAttachShader(this->Program, vertex);
  glAttachShader(this->Program, fragment);
  glLinkProgram(this->Program);
  // Print linking errors if any
  glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    return false;
  }
  // Delete the shaders as they're linked into our program now and no longer necessery
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return true;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  LoadAndCompile(vertexPath, fragmentPath);
}

// Uses the current shader
void Shader::Use()
{
  glUseProgram(this->Program);
}

void Shader::SetMat4F(const std::string& id, const Matrix4x4& mat)
{
  GLint loc = glGetUniformLocation(Program, id.c_str());
  glUniformMatrix4fv(loc, 1, GL_TRUE, mat.m2);
}

void Shader::SetVec3(const std::string &name, const Vector3& value) const
{
  glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, value.m);
}

void Shader::SetVec4(const std::string &name, const Vector4& value) const
{
  glUniform4fv(glGetUniformLocation(Program, name.c_str()), 1, value.m);
}

void Shader::SetFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
}

void Shader::SetInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}

/*

glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model.m2);
glUniformMatrix4fv(viewLoc, 1, GL_TRUE, m_camera.m_ViewMatrix.m2);
glUniformMatrix4fv(projLoc, 1, GL_TRUE, m_camera.m_PerspectiveMatrix.m2);
*/