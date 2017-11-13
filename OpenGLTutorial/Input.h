#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <map>

#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "BBEMath.h"
#include "Key.h"
#include "SystemComponentBase.h"

namespace IZE
{
  class Input : public SystemComponentBase
  {
  public:
    static Input* SingletonPtr;
    Input(void);
    ~Input(void);

    void Load(void);
    void Init(void);
    void Update(void);

    bool IsKeyReleased(KEY)              const;
    bool IsKeyDown(KEY)              const;
    bool IsKeyUp(KEY)              const;
    bool IsKeyTriggered(KEY)              const;
    bool IsMouseButtonReleased(MOUSE_BUTTON)     const;
    bool IsMouseButtonDown(MOUSE_BUTTON)     const;
    bool IsMouseButtonUp(MOUSE_BUTTON)     const;
    bool IsMouseButtonTriggered(MOUSE_BUTTON)     const;
    bool IsMouseWheelScrolledUp(void)             const { return m_ScrollUp; }
    bool IsMouseWheelScrolledDown(void)             const { return m_ScrollDown; }
    bool IsControllerPluggedIn(void)             const { return m_JoystickPluggedIn; }
    bool IsControllerButtonDown(ControllerButton) const;
    bool IsControllerButtonUp(ControllerButton) const;
    bool IsControllerButtonReleased(ControllerButton) const;
    bool IsControllerButtonTriggered(ControllerButton) const;
    float GetControllerAxesValue(ControllerAxes)   const;
    Vector2 GetCursorWindowCoords(void)             const;
    Vector3 GetCursorViewCoords(void)             const;
    Vector3 GetCursorWorldCoords(void)             const;
    void SetCursorWindowCoords(const Vector2&);

    Vector2     m_CursorDelta;
    Vector3     m_CursorNDCPos;
    Vector2     m_CursorViewPortPos;
    Vector3     m_CursorViewFramePos;
    Vector3     m_CursorWorldFramePos;
    std::string m_JoystickName;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void cursor_enter_callback(GLFWwindow* window, int entered);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

  private:

    void UpdateCursorPositions(void);
    void UpdateJoystickControls(void);

    bool                            m_CurrentKeys[GLFW_KEY_LAST];
    bool                            m_OldKeys[GLFW_KEY_LAST];
    bool                            m_CurrentMouse[MOUSE_BUTTON_LAST];
    bool                            m_OldMouse[MOUSE_BUTTON_LAST];
    bool                            m_ScrollUp;
    bool                            m_ScrollDown;
    bool                            m_JoystickPluggedIn = false;
    bool                            m_CurrentControllerButtons[(int)ControllerButton::COUNT];
    bool                            m_OldControllerButtons[(int)ControllerButton::COUNT];
    Vector2                         m_PrevCursorViewPortPos;
    std::map<ControllerAxes, float> m_ControllerAxesValues;
  };
}
