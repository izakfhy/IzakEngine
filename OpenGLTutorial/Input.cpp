
#include "Input.h"
#include "Globals.h"
#include "GraphicsEngine.h"
#include "Camera.h"


IZE::Input* IZE::Input::SingletonPtr = nullptr;
namespace IZE
{
  Input::Input()
  {
    SingletonPtr = this;
  }

  Input::~Input()
  {

  }

  void Input::Load()
  {
  }

  void Input::Init()
  {
    std::memset(m_CurrentKeys, 0, KEY_LAST);
    std::memset(m_OldKeys, 0, KEY_LAST);
    std::memset(m_CurrentMouse, 0, MOUSE_BUTTON_LAST);
    std::memset(m_OldMouse, 0, MOUSE_BUTTON_LAST);
    std::memset(m_OldControllerButtons, 0, (int)ControllerButton::COUNT);
    std::memset(m_CurrentControllerButtons, 0, (int)ControllerButton::COUNT);
    m_ScrollDown = m_ScrollUp = false;

    if (glfwJoystickPresent(GLFW_JOYSTICK_1))
    {
      m_JoystickPluggedIn = true;
      std::string msg = glfwGetJoystickName(GLFW_JOYSTICK_1);
      m_JoystickName = msg;
      msg += " detected!";
      //CONSOLE_MSG(msg);
    }
    else
    {
      m_JoystickPluggedIn = false;
      printf("No Controller Detected!\n");
    }

    //LOG_MSG("Input Component Initialized.");
  }

  void Input::Update()
  {
    //ProfileThis;
    UpdateCursorPositions();

    // update old keys to detect triggering
    for (int i = 0; i < KEY_LAST; ++i) m_OldKeys[i] = m_CurrentKeys[i];
    for (int i = 0; i < MOUSE_BUTTON_LAST; ++i) m_OldMouse[i] = m_CurrentMouse[i];
    for (int i = 0; i < (int)ControllerButton::COUNT; ++i) m_OldControllerButtons[i] = m_CurrentControllerButtons[i];
    m_ScrollDown = m_ScrollUp = false;

    // poll for new key input
    UpdateJoystickControls();
    glfwPollEvents();

    // update cursor delta
    m_CursorDelta = m_CursorViewPortPos - m_PrevCursorViewPortPos;
    m_PrevCursorViewPortPos = m_CursorViewPortPos;
  }

  bool Input::IsKeyReleased(KEY key) const
  {
    return !m_CurrentKeys[key] && m_OldKeys[key];
  }

  bool Input::IsKeyDown(KEY key) const
  {
    return m_CurrentKeys[key];
  }

  bool Input::IsKeyUp(KEY key) const
  {
    return !m_CurrentKeys[key];
  }

  bool Input::IsKeyTriggered(KEY key) const
  {
    return m_CurrentKeys[key] && !m_OldKeys[key];
  }

  bool Input::IsMouseButtonReleased(MOUSE_BUTTON key) const
  {
    return !m_CurrentMouse[key] && m_OldMouse[key];
  }

  bool Input::IsMouseButtonDown(MOUSE_BUTTON key) const
  {
    return m_CurrentMouse[key];
  }

  bool Input::IsMouseButtonUp(MOUSE_BUTTON key) const
  {
    return !m_CurrentMouse[key];
  }

  bool Input::IsMouseButtonTriggered(MOUSE_BUTTON key) const
  {
    return m_CurrentMouse[key] && !m_OldMouse[key];
  }

  bool Input::IsControllerButtonDown(ControllerButton b) const
  {
    return m_CurrentControllerButtons[(int)b];
  }

  bool Input::IsControllerButtonUp(ControllerButton b) const
  {
    return !m_CurrentControllerButtons[(int)b];
  }

  bool Input::IsControllerButtonReleased(ControllerButton b) const
  {
    return !m_CurrentControllerButtons[(int)b] && m_OldControllerButtons[(int)b];
  }

  bool Input::IsControllerButtonTriggered(ControllerButton b) const
  {
    return m_CurrentControllerButtons[(int)b] && !m_OldControllerButtons[(int)b];
  }

  float Input::GetControllerAxesValue(ControllerAxes a) const
  {
    return m_ControllerAxesValues.at(a);
  }

  Vector2 Input::GetCursorWindowCoords() const
  {
    return m_CursorViewPortPos;
  }

  Vector3 Input::GetCursorViewCoords() const
  {
    return Vector3();
    //return{ m_CursorViewFramePos.x, m_CursorViewFramePos.y, -g_camera->m_Near };
  }

  Vector3 Input::GetCursorWorldCoords() const
  {
    return m_CursorWorldFramePos;
  }

  void Input::SetCursorWindowCoords(const Vector2& v)
  {
    m_PrevCursorViewPortPos = v;
    //glfwSetCursorPos(g_engine->m_GLFWwindow, v.x, v.y);
  }

  void Input::UpdateCursorPositions(void)
  {
    // get Cursor Viewport pos
     double xpos, ypos;
    glfwGetCursorPos(g_graphics->m_window, &xpos, &ypos);
    m_CursorViewPortPos = { (float)xpos, (float)ypos };

    // get Cursor NDC pos
    Vector2 WinSz = g_graphics->GetWindowSize();
    float NDCx = (2.0f * m_CursorViewPortPos.x) / WinSz.x - 1.0f;
    float NDCy = 1.0f - (2.0f * m_CursorViewPortPos.y) / WinSz.y;
    m_CursorNDCPos = { NDCx, NDCy, -1 };

    // get Cursor Viewframe pos
    Vector4 View = g_camera.m_PerspectiveMatrix.Inverse() * Vector4(NDCx, NDCy, -1, 1);
    m_CursorViewFramePos = { View.x, View.y, View.z };

    // get Cursor Worldframe pos
    Vector4 World = g_camera.m_ViewMatrix.OrthoNormalInverse() * Vector4(m_CursorViewFramePos, 1);
    m_CursorWorldFramePos = { World.x, World.y, World.z };
  }

  void Input::UpdateJoystickControls(void)
  {
    // detect if existing controller is plugged out
    /* const double CHECK_CONTROLLER_INTERVAL = 1;
    static double Check_Timer = 0;
    Check_Timer += g_dt;

    if (Check_Timer >= CHECK_CONTROLLER_INTERVAL)
    {
    if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_FALSE && m_JoystickPluggedIn)
    {
    m_JoystickPluggedIn = false;
    CONSOLE_MSG("Joystick Unplugged!");
    if (g_gsm->GetCurState() == "test")
    g_level->GetPauseMenu()->TriggerPause();
    }

    // detect if new controller is plugged in
    if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE && !m_JoystickPluggedIn)
    {
    m_JoystickPluggedIn = true;
    CONSOLE_MSG("Joystick Detected!");
    if (g_gsm->GetCurState() == "test")
    g_level->GetPauseMenu()->TriggerPause();
    }

    //Check_Timer = 0;
    }
    */

    /*if (!m_JoystickPluggedIn) return;

    // poll for controller axis sticks
    int AxesCount;
    const float* Axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &AxesCount);

    if (!AxesCount)
    {
    m_JoystickPluggedIn = false;
    std::string msg = "Error getting controller stick axis' states for: " + m_JoystickName;
    CONSOLE_MSG(msg);
    return;
    }

    // save controller axis stick values
    m_ControllerAxesValues[LeftStickHorizontal] = Axes[0];
    m_ControllerAxesValues[LeftStickVertical] = Axes[1];
    m_ControllerAxesValues[LeftTrigger] = Axes[2];
    m_ControllerAxesValues[RightTrigger] = -Axes[2];
    m_ControllerAxesValues[RightStickVertical] = Axes[3];
    m_ControllerAxesValues[RightStickHorizontal] = Axes[4];

    // poll for controller buttons
    int ButtonCount;
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &ButtonCount);
    if (!ButtonCount)
    {
    m_JoystickPluggedIn = false;
    std::string msg = "Error getting controller buttons' states for: " + m_JoystickName;
    CONSOLE_MSG(msg);
    return;
    }

    // save controller button values
    for (int i = 0; i < (int)ControllerButton::COUNT; ++i)
    {
    m_CurrentControllerButtons[i] = buttons[i] ? true : false;
    }*/
  }

  //bool LevelEditor::PointerInTwBar(TwBar& bar)
  //{
  //	int pos[2];
  //	int size[2];
  //	TwGetParam(&bar, NULL, "position", TW_PARAM_INT32, 2, pos);
  //	TwGetParam(&bar, NULL, "size", TW_PARAM_INT32, 2, size);
  //	int mouseX = Input::mouse->GetX();
  //	int mouseY = Input::mouse->GetY();
  //
  //	if (mouseX < pos[0] ||
  //		mouseX > pos[0] + size[0] ||
  //		mouseY < pos[1] ||
  //		mouseY > pos[1] + size[1])
  //		return false;
  //	return true;
  //}

  void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    // unknown key
    if (key == GLFW_KEY_UNKNOWN)
    {
      // CONSOLE_MSG("Key : " << key << " is unavailable");
    }
    else
    {
      // detect key press
      if (action == GLFW_PRESS || action == GLFW_REPEAT)
      {
        g_input->m_CurrentKeys[key] = true;
        g_input->m_CurrentKeys[mods] = true;
      }

      // detect key release
      else if (action == GLFW_RELEASE)
      {
        g_input->m_CurrentKeys[key] = false;
        g_input->m_CurrentKeys[mods] = false;
        g_input->m_OldKeys[key] = true;
        g_input->m_OldKeys[mods] = true;
      }
    }
  }

  void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
  {

    // unknock key
    if (button == GLFW_KEY_UNKNOWN)
    {
      // CONSOLE_MSG("Key : " << button << " is unavailable");
    }
    else
    {
      // detect key press
      if (action == GLFW_PRESS)
      {
        g_input->m_CurrentMouse[button] = true;
      }

      // detect key release
      else if (action == GLFW_RELEASE)
      {
        g_input->m_CurrentMouse[button] = false;
        g_input->m_OldKeys[button] = true;
      }
    }
  }

  void Input::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
  {

  }

  void Input::cursor_enter_callback(GLFWwindow* window, int entered)
  {
    if (entered)
    {
    }
    else
    {
    }
  }

  void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
  {

    if (yoffset > 0)
    {
      // Scroll Up
      g_input->m_ScrollUp = true;
    }
    else if (yoffset < 0)
    {
      // Scroll Down
      g_input->m_ScrollDown = true;
    }
  }
}

