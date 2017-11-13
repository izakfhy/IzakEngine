#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GL/glut.h"
#include <memory>
#include <vector>
#include "Lights.h"
#include "Texture.h"
#include "Camera.h"
#include "SystemComponentBase.h"
#include "GameObject.h"

class Picker;
class GLFWwindow;
class FrameBufferbase;
class IZE::Texture;
class Shader;
class Model;
class ModelComponent;
class Gizmo;

namespace IZE
{
  class GraphicsEngine : public SystemComponentBase
  {
  public:
    unsigned int m_width;
    unsigned int m_height;

    GLFWwindow*         m_window;
    GLuint              m_mainVBO;
    FrameBufferbase*    m_mainFrameFBO = nullptr;
    GLuint              m_VAO, m_EBO,m_fuillScreenVAO;
    std::shared_ptr<Shader>            m_shaderProg;
    std::shared_ptr<Shader>            m_finalShader;
    GLuint              m_mainshaderProgram;
    std::shared_ptr<Shader>              m_lampShader;
    unsigned int        m_lightVAO;
    GLuint              m_lightUBO;
    IZE::Camera         m_camera;

    std::shared_ptr<IZE::Texture> sampletext;
    std::shared_ptr<IZE::Texture> samplespectext;

    Gizmo *  m_gizmo;
    Picker* m_picker;
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;
    //tests
    std::vector<Matrix4x4> m_testboxes;

    std::vector<PointLight> m_pointlights;

    GLuint CompileShader();
    bool IZECreateWindow();
    bool SetUpMainVBO();
    void LoadAndCreateShaders();
    void CreateSampleLightSource();
    void RenderGizmos();
    void SendLightData();
    void RenderFinalPass();
  public:

    static GraphicsEngine* SingletonPtr;

    GraphicsEngine();
    ~GraphicsEngine();

    void Load();
    void Init();
    void Free();
    void Unload();
    void Render();
    void Update();

    Vector2 GetWindowSize() const;
    //void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
  };
}
