/* Start Header ***************************************************************/
/*!
\file   Engine.cpp
\author Roger Law Fu Yang, rogerlaw.fuyang, 390004713
\par    rogerlaw.fuyang\@digipen.edu
\date   December 4, 2015

Project Name: Trojan XL

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
// std libraries
#include <cstdlib>
#include <iostream>

// user libraries
#include "IZEngine.h"


#include "Globals.h"
/*#include "GameStateManager.h"
#include "MessageManager.h"
#include "GameProgress.h"
#include "Factory.h"
#include "Level.h"
#include "Editor.h"
#include "TextBoxManager.h"
#include "Profiler.h"*/
#include "SystemComponentHeaders.h"

#include <stdlib.h>
#include <time.h>

double g_dt = 1.0 / 60;
double g_trueDt = 1.0 / 60;
double g_cappedDt = 1.0 / 60;
double g_TrueGameTime = 0;
double g_GameTime = 0;

using namespace IZE;

IZEngine* IZEngine::SingletonPtr = nullptr;

IZEngine::IZEngine()
{
  SingletonPtr = this;
  //static MessageManager<std::string> mm;
  //static GameProgress gp;
  srand((unsigned)time(NULL));
}

IZEngine::~IZEngine()
{

}

void IZEngine::Initialize(int argc, char* argv[])
{
 // Deserialize();
 // m_MemoryManager.AllocateMainMemory(m_MemoryToAllocate);
  //m_Logger = New(SystemLogger);

  printf("Izak Engine 2\n");
  //InitGLFWandWindow();
  //InitGLEW();

  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("GLSL Version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  InitializeSystemComponents();
  PrepareCallbackFunctions();
 // glfwSwapInterval(m_Vsync);
}

void IZEngine::ShutDown()
{
  
  //glfwDestroyWindow(m_GLFWwindow);
  //glfwTerminate();

  g_graphics->Free();
  g_graphics->Unload();

  //g_editor->AutoSaveFilePath();
  //g_factory->SaveSceneToFile(g_level->m_lastEditedLevelPath);

  // Free all system components from Memory Manager
  for (auto& elem : m_SystemComponents)
  {
    delete elem.second;
    //SAFE_DELETE(elem.second);
  }
  
}

void IZEngine::InitializeSystemComponents()
{
  //LOG_MSG("Initializing System Componenets...");
  using namespace IZE;
#define REGISTER_SYSTEM_COMPONENT_NAME(x) m_SystemComponents[SYS_CMP::x] = new x(); m_SystemComponents[SYS_CMP::x]->Load();
#include "SystemComponentNames.h"
#undef REGISTER_SYSTEM_COMPONENT_NAME

#define REGISTER_SYSTEM_COMPONENT_NAME(x) m_SystemComponents[SYS_CMP::x]->Init();
#include "SystemComponentNames.h"
#undef REGISTER_SYSTEM_COMPONENT_NAME

 // LOG_MSG("System Componenets Initialized!");
}


void CapFrameRates(double StartFrameTime)
{
  const int HighFPSCap = 61;
  const int LowFPSCap = 30;
  const double HighFPSCapInv = 1.0 / HighFPSCap;
  const double LowFPSCapInv = 1.0 / LowFPSCap;

  // Sleep thread to HighFPSCap
  double TimeToSleep = (HighFPSCapInv - g_trueDt) * 1000;
  if (TimeToSleep > 0)
  {
    double TimeBeforeSleep = glfwGetTime();
    Sleep((DWORD)TimeToSleep);
    g_trueDt += glfwGetTime() - TimeBeforeSleep;
  }

  // cap dt to LowFPSCap
  g_cappedDt = min(max(g_trueDt, HighFPSCapInv), LowFPSCapInv);
}

void IZEngine::Run()
{
  //LOG_MSG("Engine Running");
  /*if (m_StartLevel == "./Resources/Levels/Level_MainMenu/Level MainMenu.xml")
    g_gsm->PushState(MAINMENU_STATE, m_StartLevel);
  else if (m_StartLevel == "./Resources/Levels/Level_SplashScreen/Level SplashScreen.xml")
    g_gsm->PushState(SPLASHSCREEN_STATE, m_StartLevel);
  else
    g_gsm->PushState(START_STATE, m_StartLevel);

  g_text->showConsole = false;
  g_text->SetConsoleScale(0.5f);*/
  while (!glfwWindowShouldClose(g_graphics->m_window) && !m_StopEngine)
  {
    // if (g_input->IsKeyTriggered(KEY_T)) ToggleFullScreen();

    double StartFrameTime = glfwGetTime();

    // close/open debug text
    // if (g_input->IsKeyTriggered(KEY_GRAVE_ACCENT))
    //   g_text->showConsole = g_text->showConsole ^ 1;
    //g_audioSystem->Update();

    if (g_input->IsKeyTriggered(KEY_ESCAPE))
      m_StopEngine = true;
    // run game
    MainGameLoop();
    

    // Update Uncapped fps
    g_trueDt = glfwGetTime() - StartFrameTime;
    g_TrueGameTime += g_trueDt;
    m_RealFPS = (0.9 * m_RealFPS) + (0.1 * (1.0 / g_trueDt));

    // cap low and high g_dt
    CapFrameRates(StartFrameTime);
    m_CappedFPS = (0.9 * m_CappedFPS) + (0.1 * (1.0 / g_cappedDt));

    // apply game speed modifier
    g_dt = m_FpsScale * g_cappedDt;
    g_GameTime += g_dt;

    static double AvgFPSUpdateTimer = 0;
    AvgFPSUpdateTimer += g_trueDt;
    if (AvgFPSUpdateTimer > 0.5)
    {
     /* g_text->AddDCTextBox("FPS", "Real_FPS: " + std::to_string((int)m_RealFPS));
      g_text->AddDCTextBox("CapFPS", "Capped_Fps: " + std::to_string((int)m_CappedFPS));
      g_text->AddDCTextBox("GameFPS", "Game_Fps: " + std::to_string(1 / g_dt));
      g_text->AddDCTextBox("MemAlloc", "Memory Allocations: " + std::to_string(g_memory->m_Stats.Allocates));

      BBE::Profiler::Profile** obj = g_profiler->GetTopThree();
      if (obj[0]) g_text->AddDCTextBox("Largest1 usage:", obj[0]->m_name + std::to_string(obj[0]->m_prevTime));
      if (obj[1]) g_text->AddDCTextBox("Largest2 usage:", obj[1]->m_name + std::to_string(obj[1]->m_prevTime));
      if (obj[2]) g_text->AddDCTextBox("Largest3 usage:", obj[2]->m_name + std::to_string(obj[2]->m_prevTime));
      */
      AvgFPSUpdateTimer = 0;
    }

    // update profiler to grab g_dt of previous frame
    //g_profiler->LoopUpdate();

    // intermittently join memory manager's memory blocks 
   /* static double mergeTimer = 1;
    mergeTimer -= g_trueDt;
    if (mergeTimer <= 0)
    {
      g_memory->CombineAdjacentBlocks();
      mergeTimer = 10;
    }*/
  }

 // LOG_MSG("Engine Stopped");
}

void IZEngine::PrepareCallbackFunctions()
{
  // Prepare GLFW callback functions
 // glfwSetWindowCloseCallback(m_GLFWwindow, window_close_callback);
 // glfwSetWindowSizeCallback(m_GLFWwindow, window_size_callback);
 // glfwSetWindowPosCallback(m_GLFWwindow, window_pos_callback);
 // glfwSetWindowIconifyCallback(m_GLFWwindow, window_minimized_callback);
 // glfwSetWindowFocusCallback(m_GLFWwindow, window_focus_callback);
 // glfwSetWindowRefreshCallback(m_GLFWwindow, window_refresh_callback);
 // glfwSetMonitorCallback(monitor_callback);
  glfwSetKeyCallback(g_graphics->m_window, Input::key_callback);
  glfwSetCursorEnterCallback(g_graphics->m_window, Input::cursor_enter_callback);
  glfwSetCursorPosCallback(g_graphics->m_window, Input::cursor_pos_callback);
  glfwSetMouseButtonCallback(g_graphics->m_window, Input::mouse_button_callback);
  glfwSetScrollCallback(g_graphics->m_window, Input::scroll_callback);
  //glfwSetCharCallback(m_GLFWwindow, GLFW_char_callback);

  //LOG_MSG("GLFW Callback Functions Initialized");
}

void IZEngine::MainGameLoop()
{
  static bool m_frameByFrame = false;
  //m_ShowCursor = true;
  g_input->Update();
  g_CompGM->Update();
  /*g_gsm->Update();

  if (g_input->IsKeyTriggered(GLFW_KEY_LEFT_BRACKET) && g_gsm->GetCurStatePause() == true)
  {
    m_frameByFrame = true;
  }

  if (g_gsm->GetCurStatePause() == false || m_frameByFrame == true)
  {
    g_componentGM->Update();
  }
  g_editor->Update();*/
#ifndef _FINAL
 // g_lua->Update();
#endif
  /*if (m_PrevShowCursor != m_ShowCursor)
  {
    glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, m_ShowCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
    m_PrevShowCursor = m_ShowCursor;
  }*/
  //g_audioSystem->Update();

  g_graphics->Update();

  m_frameByFrame = false;
}


void IZEngine::SetWindowTitle(const std::string& s)
{
  glfwSetWindowTitle(m_GLFWwindow, s.c_str());
}

void IZEngine::SetMinimizedWindow(bool b)
{
  b ? glfwIconifyWindow(m_GLFWwindow) : glfwRestoreWindow(m_GLFWwindow);
}

void IZEngine::ToggleFullScreen(bool ForVsync)
{
  if (!ForVsync)
    m_FullScreen = !m_FullScreen;

  // unload editor
 /* g_editor->CleanUp();
  g_editor->DestoryAllBars();
  g_editor->TerminateAnttweakBar();*/

  // Destroy old window
  glfwDestroyWindow(m_GLFWwindow);

  // Make new window
  //PrepareWindowProperties();

  // Create new window
  m_CurrentScreenResolution = m_FullScreen ? &m_FullScreenResolution : &m_WindowResolution;
  m_GLFWwindow = glfwCreateWindow(
    (int)m_CurrentScreenResolution->x,
    (int)m_CurrentScreenResolution->y,
    m_WindowTitle.c_str(),
    m_FullScreen ? glfwGetPrimaryMonitor() : nullptr,
    nullptr);

  // set up window context
  glfwMakeContextCurrent(m_GLFWwindow);
  //PrepareCallbackFunctions();

  // reload graphics engine
  //->m_Reloading = true;
  //g_graphics->Reload();
  //g_graphics->m_Reloading = false;

  // Enable or disable vsync
  glfwSwapInterval(m_Vsync);

  // Show cursor
  ShowCursor(m_ShowCursor);

  // load Editor
 // g_editor->Load();

  printf("Switched to Fullscreen Mode : %d\n", m_FullScreen);
  printf("New Resolution : (%d, %d)\n", (int)m_CurrentScreenResolution->x, (int)m_CurrentScreenResolution->y);
}

void IZEngine::ShowCursor(bool b)
{
  glfwSetInputMode(m_GLFWwindow, GLFW_CURSOR, b ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
  m_ShowCursor = b;
}

void IZEngine::StopEngine(const std::string& file, long line)
{
  m_StopEngine = true;
}

Vector2 IZEngine::GetWindowSize() const
{
  int x, y;
  glfwGetWindowSize(m_GLFWwindow, &x, &y);
  //ASSERTMSG(x && y, "Failed to get Window Size!");
  return Vector2((float)x, (float)y);
}

Vector2 IZEngine::GetFrameBufferSize() const
{
  int width, height;
  glfwGetFramebufferSize(m_GLFWwindow, &width, &height);
 // ASSERTMSG(width && height, "Failed to get Framebuffer Size!");
  return Vector2((float)width, (float)height);
}

Vector2 IZEngine::GetWindowPos() const
{
  int xpos, ypos;
  glfwGetWindowPos(m_GLFWwindow, &xpos, &ypos);
  return Vector2((float)xpos, float(ypos));
}

WindowFrame IZEngine::GetWindowFrameSize() const
{
  WindowFrame w;
  glfwGetWindowFrameSize(m_GLFWwindow, &w.left, &w.top, &w.right, &w.bottom);
  return w;
}


