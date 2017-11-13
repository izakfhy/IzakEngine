#pragma once
#include <string>
#include <map>


#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "BBEMath.h"
#include "SystemComponentBase.h"

// lib includes
#ifdef _DEBUG
#pragma comment (lib, "glfw3d.lib")
#pragma comment (lib, "glew32sd.lib")
#else
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "glew32s.lib")
#endif
#pragma comment (lib, "opengl32.lib")


// Forward Declarations
class SystemComponentBase;

namespace IZE
{

  struct WindowFrame
  {
    int left, top, right, bottom;
  };


  class IZEngine
  {
  public:
    IZEngine(void);
    ~IZEngine(void);
    void Initialize(int argc, char* argv[]);
    void Run(void);
    void ShutDown(void);

    // Settors
    void SetWindowTitle(const std::string& s);
    void SetMinimizedWindow(bool);
    void ToggleFullScreen(bool ForVsync = false);
    void ShowCursor(bool);
    void StopEngine(const std::string&, long);

    // Gettors
    static IZEngine* SingletonPtr;
    GLFWmonitor* GetPrimaryMonitor(void)       { return glfwGetPrimaryMonitor(); }
    GLFWwindow* GetGLFWwindow(void) const { return m_GLFWwindow; }
    void* GetWindowPointer(void)       { return glfwGetWindowUserPointer(m_GLFWwindow); }
    bool IsFullScreen(void) const { return m_FullScreen; }
    bool IsVsyncEnabled(void) const { return m_Vsync; }
    int IsWindowMinimized(void) const { return glfwGetWindowAttrib(m_GLFWwindow, GLFW_ICONIFIED); }
    int IsWindowFocused(void) const { return glfwGetWindowAttrib(m_GLFWwindow, GLFW_FOCUSED); }
    double GetGameTime(void) const { return glfwGetTime(); }
    Vector2 GetOpenGLVersion(void) const { return m_GLFW_CONTEXT_VERSION; }
    Vector2 GetWindowResolution(void) const { return m_WindowResolution; }
    Vector2 GetWindowSize(void) const;
    Vector2 GetFrameBufferSize(void) const;
    Vector2 GetWindowPos(void) const;
    WindowFrame GetWindowFrameSize(void) const;


    bool                                    m_ShowCursor = true;
    bool                                    m_UseMemoryManager = true;
    bool                                    m_Vsync = true;
    bool                                    m_StopEngine = false;
    int                                     m_AntiAliasingAmount = 1;
    int                                     m_MemoryToAllocate = 256;
    double                                  m_RealFPS = 60;
    double                                  m_CappedFPS = 60;
    Vector2                                 m_GLFW_CONTEXT_VERSION = { 3, 3 };
    Vector2                                 m_WindowResolution = { 1280, 720 };
    Vector2                                 m_FullScreenResolution = { 1280, 720 };
    Vector2*                                m_CurrentScreenResolution = nullptr;
    GLFWwindow*                             m_GLFWwindow = nullptr;
    std::string                             m_WindowTitle = "TrojanXL";
    std::map<SYS_CMP, SystemComponentBase*> m_SystemComponents;

    //MemoryManager                           m_MemoryManager;
    //SystemLogger*                           m_Logger;
    //Level*                                  m_Level = nullptr;
    float                                   m_ControllerDeadZone = 0.25f;
    float                                   m_ControllerDeadZoneInv = 1.f / m_ControllerDeadZone;
    float                                   m_ControllerSensitivity = 0.02f;
    float                                   m_MinControllerSensitivity = 0.01f;
    float                                   m_MaxControllerSensitivity = 0.03f;
    float                                   m_MouseSensitivity = 0.0004f;
    float                                   m_MinMouseSensitivity = 0.0002f;
    float                                   m_MaxMouseSensitivity = 0.0006f;
    float                                   m_FpsScale = 1.f;
  private:
    void Deserialize(void);
    void InitializeSystemComponents(void);
    void InitGLFWandWindow(void);
    void InitGLEW(void);
    void PrepareWindowProperties(void);
    void CreateOpenGLWindow(void);
    void PrepareCallbackFunctions(void);
    void MainGameLoop(void);

    bool        m_FullScreen = false;
    bool        m_PrevShowCursor = true;
    std::string m_StartLevel;
  };
}