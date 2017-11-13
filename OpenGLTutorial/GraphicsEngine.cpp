#include <iostream>
#include "GraphicsEngine.h"
#define GLEW_STATIC
#include "GLFW/glfw3.h"

#include "GL/glut.h"
#include <vector>
#include <string>
#include <fstream>
#include "FrameBuffers.h"
#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "RefelectionUtils.h"
#include "Factory.h"
#include "Globals.h"
#include "ComponentGM.h"
#include "AllComponents.h"
#include "Gizmo.h"
#include "Picker.h"
#include "SystemComponentHeaders.h"

#pragma comment(lib,"lib/glfw3")
#pragma comment(lib,"lib/glew32s")

DEFINE_META_POD( int );
DEFINE_META_POD( char );
DEFINE_META_POD( char * );
DEFINE_META_POD( float );
DEFINE_META_POD( bool );
DEFINE_META_POD(unsigned);
DEFINE_META_POD( double );
DEFINE_META_POD( std::string );

///////////////////////////////////////////////////////////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  // When a user presses the escape key, we set the WindowShouldClose property to true, 
  // closing the application
  //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    //glfwSetWindowShouldClose(window, GL_TRUE);
}

///////////////////////////////////////////////////////////////////////////////////////
const  // Set up vertex data (and buffer(s)) and attribute pointers
float vertices[] = {
  // positions          // normals           // texture coords
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};
const GLuint indices[] = {  // Note that we start from 0!
  0, 1, 3, // First Triangle
  1, 2, 3  // Second Triangle
};

// The fullscreen quad's FBO
static const GLfloat g_quad_vertex_buffer_data[] = {
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  -1.0f, 1.0f, 0.0f,
  -1.0f, 1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  1.0f, 1.0f, 0.0f,
};
/////////////////////////testing stuff////////////////////////////////////////////

Vector3 m_camera(0, 0, 3);
Vector3 m_lookat(0, 0, 0);
Vector3 m_up(0, 1, 0);
Vector3 m_dir;

/////////////////////////testing stuff////////////////////////////////////////////
IZE::GraphicsEngine* IZE::GraphicsEngine::SingletonPtr = nullptr;
namespace IZE
{


#define PRINT_MEMBERS( TYPE ) \
  PrintMembers<TYPE>( #TYPE )


  void reflectTest()
  {
   /* int x = 5;

    refvar refv;
    refv = x;

    const char *c = "Stringdasdd";
    var v = c;
    refv = v;

    v.Serialize(std::cout);

   

    Model *m = new Model("C:\\Users\\Izak\\Documents\\Visual Studio 2013\\Projects\\OpenGLTutorial\\OpenGLTutorial\\Resource\\Models\\Nanosuit\\nanosuit.obj");
   
    refvar object = *m;
    object.Serialize(std::cout);
    */
    

  }

  GraphicsEngine::GraphicsEngine() :
    m_width(800),
    m_height(600),
    m_window(nullptr)
  {
    SingletonPtr = this;
    m_mainFrameFBO = new IntermediateFBO();
    m_picker = new Picker();

  }


  GraphicsEngine::~GraphicsEngine()
  {
    delete m_picker;
    delete m_mainFrameFBO;
    //delete m_shaderProg;
    //delete m_lampShader;
  }

  bool GraphicsEngine::IZECreateWindow()
  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(m_width, m_height, "LearnOpenGL", nullptr, nullptr);

    if (m_window == nullptr)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }
    //glfwSetKeyCallback(m_window, key_callback);

    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
      std::cout << "Failed to initialize GLEW" << std::endl;
      return false;
    }

    glViewport(0, 0, m_width, m_height);
  }

  void GraphicsEngine::Load()
  {
    IZECreateWindow();
    LoadAndCreateShaders();
  }
  void GraphicsEngine::CreateSampleLightSource()
  {

    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the correct data.
    m_mainFrameFBO->Bind();
    // set the vertex attributes (only position data for our lamp)
    /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   
    glBindVertexArray(0); // Unbind VAO

    // don't forget to 'use' the corresponding shader program first (to set the uniform)
    //m_lampShader = new Shader("lamp.vert", "lamp.frag");
    m_lampShader = IZE::ResourceManager::GetInstance()->GetResource<Shader>("lamp");
  }

  void GraphicsEngine::RenderGizmos()
  {
    if (m_gizmo->GetTarget() == nullptr)
      return;

    m_lampShader->Use();
    //m_lampShader->SetVec3("objectColor", Vector3(1.0f, 0.5f, 0.31f));
    //m_lampShader->SetVec3("lightColor", Vector3(1.0f, 1.0f, 1.0f));

    m_lampShader->SetMat4F("view", m_camera.m_ViewMatrix);
    m_lampShader->SetMat4F("projection", m_camera.m_PerspectiveMatrix);

    for (int i = 0; i < 3; ++i)
    {
      auto elem = m_gizmo->m_gizmos[i];
      Transform* cmp = elem->GetComponent<Transform>();
      ModelComponent* mdl = elem->GetComponent<ModelComponent>();
      m_lampShader->SetMat4F("model", cmp->GetModelMat());
      if (i == 0)
        m_lampShader->SetVec4("objectColor", Vector4(1,0,0));
      if (i == 1)
        m_lampShader->SetVec4("objectColor", Vector4(0, 1, 0));
      if (i == 2)
        m_lampShader->SetVec4("objectColor", Vector4(0, 0, 1));
      mdl->Draw();
    }

    /*
    for (int i = 0; i < m_pointlights.size(); ++i)
    {

      Matrix4x4 model;
      model.MakeIdentity();
      model = model.MakeTranslationMatrix(m_pointlights[i].m_pos.x, m_pointlights[i].m_pos.y, m_pointlights[i].m_pos.z);
      model = model* scale;

      // view/projection transformations
      m_lampShader->SetMat4F("model", model);
      m_lampShader->SetMat4F("view", m_camera.m_ViewMatrix);
      m_lampShader->SetMat4F("projection", m_camera.m_PerspectiveMatrix);


      glBindVertexArray(m_lightVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
    }*/

   
  }

  bool GraphicsEngine::SetUpMainVBO()
  {
    m_mainFrameFBO->Init(m_width, m_height);
    glGenBuffers(1, &m_mainVBO);

    //bind to main vao obj
    glBindVertexArray(m_VAO);

    //bind vbo
    m_mainFrameFBO->Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //bind EBO
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO


    //generate and bind fullscreen quad vao
   
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);


    m_mainFrameFBO->Unbind();
    return true;
  }


  void GraphicsEngine::LoadAndCreateShaders()
  {
    m_shaderProg = IZE::ResourceManager::GetInstance()->GetResource<Shader>("Forward");
    m_lampShader = IZE::ResourceManager::GetInstance()->GetResource<Shader>("lamp");
    m_finalShader = IZE::ResourceManager::GetInstance()->GetResource<Shader>("final"); 
    //can send at init
    //m_shaderProg->SetInt("material1.diffuse", 0);
    //m_shaderProg->SetInt("material1.specular", 1);
  }

  void GraphicsEngine::Init()
  {
    glGenVertexArrays(1, &m_VAO);
    glGenVertexArrays(1, &m_fuillScreenVAO);
    glGenBuffers(1, &m_EBO);
    SetUpMainVBO();
    CreateSampleLightSource();

    glEnable(GL_DEPTH_TEST);

    sampletext = IZE::ResourceManager::GetInstance()->GetResource<IZE::Texture>("./Resource/Textures/container2.png");

    samplespectext = IZE::ResourceManager::GetInstance()->GetResource<IZE::Texture>("./Resource/Textures/container2_specular.png");
    m_camera.m_ViewMatrix.MakeIdentity();
    m_camera.m_PerspectiveMatrix.MakeIdentity();
    m_camera.m_aspect = (float)m_width / (float)m_height;
    m_camera.m_PerspectiveMatrix.MakePerspectiveMatrix(m_camera.m_aspect, m_camera.m_near, m_camera.m_far, m_camera.m_FOV);

   /* Matrix4x4 m;
    m.MakeTranslationMatrix(0, 0, 0);
    m_testboxes.push_back(m);
    Matrix4x4 m2;
    m2.MakeTranslationMatrix(3, 1, 1);
    m_testboxes.push_back(m2);
    Matrix4x4 m3;
    m3.MakeTranslationMatrix(-3, 1, 1);
    m_testboxes.push_back(m3);
    Matrix4x4 m4;
    m4.MakeTranslationMatrix(0, 1, 3);
    m_testboxes.push_back(m4);
    Matrix4x4 m5;
    m5.MakeTranslationMatrix(-10, 1, 3);
    m_testboxes.push_back(m5);*/

   
    for (auto elem : g_CompGM->GetManager<ModelComponent>()->m_vec)
    {
      Transform* cmp = elem->GetParent()->GetComponent<Transform>();
      elem->SetShader(m_shaderProg);
      //refvar t = *elem;
      //t.Serialize(std::cout);
    }


    //m_finalShader->Use();
   // GLuint texID = glGetUniformLocation(m_finalShader->Program, "renderedTexture");

    m_shaderProg->Use();
    auto& lights_vec = g_CompGM->GetManager<PointLightComponent>()->m_vec;

    GLuint uniformBlockIndexLights = glGetUniformBlockIndex(m_shaderProg->Program, "LightBlock");
    glUniformBlockBinding(m_shaderProg->Program, uniformBlockIndexLights, 0);

    // Uniform buffer object for lights
    glGenBuffers(1, &m_lightUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightUBO);
    glBufferData(GL_UNIFORM_BUFFER, lights_vec.size() * sizeof(PointLight), NULL, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndexLights, m_lightUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    int numlights = lights_vec.size();
    m_shaderProg->SetInt("num_pt_lights", numlights);
    
    m_gizmo = new Gizmo();
    m_gizmo->Init();
    m_gizmo->SetShader(m_shaderProg);
    reflectTest();


  }

  void GraphicsEngine::Free()
  {

  }

  void GraphicsEngine::Unload()
  {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteVertexArrays(1, &m_fuillScreenVAO);
    glDeleteVertexArrays(1, &m_lightVAO);
    glDeleteBuffers(1, &m_mainVBO);
    glDeleteBuffers(1, &m_lightUBO);

    // glDeleteBuffers(1, &EBO);
    glfwTerminate();
  }

  void GraphicsEngine::SendLightData()
  {
    m_shaderProg->SetFloat("material.shininess", 64.f);

    m_shaderProg->SetVec3("Dlight.direction", Vector3(-0.2f, -1.0f, -0.3f));
    m_shaderProg->SetVec3("Dlight.ambient", Vector3(0.2f, 0.2f, 0.2f));
    m_shaderProg->SetVec3("Dlight.diffuse", Vector3(0.5f, 0.5f, 0.5f));
    m_shaderProg->SetVec3("Dlight.specular", Vector3(1.0f, 1.0f, 1.0f));

    int i = 0;
    auto& lights_vec = g_CompGM->GetManager<PointLightComponent>()->m_vec;
    m_pointlights.clear();
    for (auto elem : lights_vec)
    {
      auto transform = elem->GetParent()->GetComponent<Transform>();

      PointLight pl;
      pl.m_pos = transform->m_Pos;
      pl.m_constant = elem->Constant();
      pl.m_linear = elem->Linear();
      pl.m_quadratic = elem->Quadratic();
      pl.m_ambient = elem->Ambient();
      pl.m_diffuse = elem->Diffuse();
      pl.m_specular = elem->Specular();
      pl.m_active = elem->IsActive();

      m_pointlights.push_back(pl);

      ++i;
    }
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, m_pointlights.size() * sizeof(PointLight), &m_pointlights.front());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }
  void GraphicsEngine::Render()
  {
      // Check and call events
     // glfwPollEvents();

      // Rendering commands here
    
      glBindFramebuffer(GL_FRAMEBUFFER, m_mainFrameFBO->m_FBO);
      glViewport(0, 0, m_mainFrameFBO->m_Dimensions.x, m_mainFrameFBO->m_Dimensions.y); // Render on the whole framebuffer, complete from the lower left corner to the upper right
     
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      m_camera.Update();

      //gizmo rendering
      RenderGizmos();

      m_shaderProg->Use();
      ///////////////transformations////////////////////////////////

      SendLightData();
      /////////////////////////////transfromations////////////////////

      m_shaderProg->SetMat4F("view", m_camera.m_ViewMatrix);
      m_shaderProg->SetMat4F("projection", m_camera.m_PerspectiveMatrix);
      m_gizmo->SetTarget(g_CompGM->GetManager<ModelComponent>()->m_vec[0]->GetParent());
      m_gizmo->Update();
     
      for (auto elem : g_CompGM->GetManager<ModelComponent>()->m_vec)
      {
        if (elem->GetParent()->GetTypeName() == "Gizmo")
          continue;

        Transform* cmp = elem->GetParent()->GetComponent<Transform>();
        m_shaderProg->SetMat4F("model", cmp->GetModelMat());
        int id = elem->GetParent()->GetID();
        m_shaderProg->SetInt("objectid", id);
 
        m_shaderProg->SetMat4F("InverseTransposeMat", cmp->GetModelMat().Inverse().Transpose());
        elem->Draw();
      }
      if (g_input->IsMouseButtonDown(MOUSE_BUTTON_1))
        m_picker->ReadPixelPicking(m_mainFrameFBO->m_FBO);

      RenderFinalPass();

      // Swap the buffers
      glfwSwapBuffers(m_window);
  
  }


  void GraphicsEngine::RenderFinalPass()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

 
    m_finalShader->Use();
    glActiveTexture(GL_TEXTURE0);
    m_finalShader->SetInt("renderedTexture", 0);
    //use texture from our FBO generated in PASS 1   
    glBindTexture(GL_TEXTURE_2D, dynamic_cast<IntermediateFBO*>(m_mainFrameFBO)->m_ColorBuffer);
  
    glBindVertexArray(m_fuillScreenVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
  }

  Vector2 GraphicsEngine::GetWindowSize() const
  {
    int x, y;
    glfwGetWindowSize(m_window, &x, &y);
    //ASSERTMSG(x && y, "Failed to get Window Size!");
    return Vector2((float)x, (float)y);
  }
  void GraphicsEngine::Update()
  {
    Render();
  }
}
