#include "Camera.h"
#include "Globals.h"
#include "Input.h"
#include <iostream>

DEFINE_META(IZE::Camera)
{
  ADD_MEMBER(m_FOV, true);
  ADD_MEMBER(m_aspect, true);
  ADD_MEMBER(m_far, true);
  ADD_MEMBER(m_CameraSensitivity, true);
  ADD_MEMBER(m_pos, true);
  ADD_MEMBER(m_lookat, true);
  ADD_MEMBER(m_up, true);
  ADD_MEMBER(m_ViewMatrix,false);
  ADD_MEMBER(m_PerspectiveMatrix,false);
  ADD_MEMBER(m_ForwardVec, true);
  ADD_MEMBER(m_RightSideVec, true);
  ADD_MEMBER(m_RealUp, true);
}
namespace IZE
{



  double g_dt = 1.0 / 60;

  Camera::Camera(void)
  {
    m_CameraSensitivity = 0.01;
  }
  
  Camera::~Camera(void)
  {}

  void Camera::ResetCamera(void)
  {}

  void Camera::RotateCamera()
  {
    // Get Y Axis Rotate Matrix
    Matrix4x4 YAxisRotateMat = Matrix4x4().MakeRotateMatrix(0, g_input->m_CursorDelta.x / 2.0f, 0);

    // Get Camera right side Rotate Matrix
    Matrix4x4 CamRightAxisRotateMat = Quaternion(-g_input->m_CursorDelta.y / 2.0f, m_RightSideVec).GetMatrix();


      // Rotate lookat vector
      Vector3 LookAtToCam = m_lookat - m_pos;
      LookAtToCam = CamRightAxisRotateMat * YAxisRotateMat * Vector4(LookAtToCam, 1);
      m_lookat = m_pos + LookAtToCam;
  }


  void Camera::CameraMovement()
  {
    // hardcoded camera movement speed
    const int MovementSpeed = 10;

    if (g_input->IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) || g_input->IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
      if (g_input->IsKeyDown(KEY_LEFT_ALT) && g_input->IsKeyDown(KEY_LEFT_CONTROL))
      {
        Vector3 ForwardMovement = m_ForwardVec * g_input->m_CursorDelta.y * m_CameraSensitivity;
        m_pos -= ForwardMovement;
        m_lookat -= ForwardMovement;
      }
      else if (g_input->IsKeyDown(KEY_LEFT_ALT))
      {
        RotateCamera();
      }
      else
      {
        Vector3 XDelta = m_RightSideVec * g_input->m_CursorDelta.x;
        Vector3 YDelta = m_RealUp * g_input->m_CursorDelta.y;
        m_pos -= XDelta * m_CameraSensitivity;
        m_pos += YDelta * m_CameraSensitivity;
        m_lookat -= XDelta * m_CameraSensitivity;
        m_lookat += YDelta * m_CameraSensitivity;
      }
    }
    // get camera movement vectors
    Vector3 ForwardMovement = m_ForwardVec * MovementSpeed * (float)g_dt;
    Vector3 SideMovement = m_RightSideVec * MovementSpeed * (float)g_dt;
    Vector3 UpMovement = m_RealUp * MovementSpeed * (float)g_dt;

    float offSet = 50 * m_CameraSensitivity;
    if (g_input->IsMouseWheelScrolledUp())
    {
      m_pos += m_ForwardVec * offSet;
      m_lookat += m_ForwardVec * offSet;
    }

    if (g_input->IsMouseWheelScrolledDown())
    {
      m_pos -= m_ForwardVec * offSet;
      m_lookat -= m_ForwardVec * offSet;
    }
    if (g_input->IsKeyDown(KEY_W))
    {
      m_pos += ForwardMovement;
      m_lookat += ForwardMovement;
    }
    if (g_input->IsKeyDown(KEY_S))
    {
      m_pos -= ForwardMovement;
      m_lookat -= ForwardMovement;
    }
    if (g_input->IsKeyDown(KEY_A))
    {
      m_pos -= SideMovement;
      m_lookat -= SideMovement;
    }
    if (g_input->IsKeyDown(KEY_D))
    {
      m_pos += SideMovement;
      m_lookat += SideMovement ;
    }
  }

  //virutals
  void Camera::Update(void)
  {
    // update camera vectors
    m_ForwardVec = (m_lookat - m_pos).Normalize();
    m_RightSideVec = m_ForwardVec.Cross(m_up).Normalize();
    m_RealUp = m_RightSideVec.Cross(m_ForwardVec);

    CameraMovement();
   
    m_ViewMatrix.MakeLookat(m_pos, m_lookat, m_up);
   // m_PerspectiveMatrix.MakePerspectiveMatrix(m_aspect, m_near, m_far, m_FOV);
    //m_PerspectiveMatrix.MakePerspectiveMatrix((float)m_width / (float)m_height, 0.1f, 1000.f, 45.0f);
  }
}