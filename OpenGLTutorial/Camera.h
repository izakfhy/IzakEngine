#pragma once
#include "BBEMath.h"
#include "RefelectionUtils.h"

namespace IZE
{
  class Camera
  {
    public:
      float     m_FOV = ToRadian(45.0);
      float     m_aspect;
      float     m_near = 0.1f;
      float     m_far = 10000.f;
      float     m_CameraSensitivity = 0.1;

      Vector3   m_pos = {0,0,3};
      Vector3   m_lookat = { 0, 0, 0 };
      Vector3   m_up = {0, 1, 0};
      Matrix4x4 m_ViewMatrix;
      Matrix4x4 m_PerspectiveMatrix;
      Vector3   m_ForwardVec = { 0, 0, 1 };
      Vector3   m_RightSideVec = { 1, 0, 0 };
      Vector3   m_RealUp = { 0, 1, 0 };
      Camera(void);
      virtual ~Camera(void);

      void CameraMovement();
      void ResetCamera(void);
      void RotateCamera();
      //virutals
      virtual void Update(void);
      META_DATA(Camera);
  };

}
