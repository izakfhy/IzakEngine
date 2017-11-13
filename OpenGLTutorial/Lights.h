#pragma once

#include "BBEMath.h"
#include "FrameBuffers.h"
#include <vector>

struct AmbientLight
{
  virtual ~AmbientLight(){}

 // bool m_active = false;
 /* bool m_castShadows = true;
  float m_intensity = 0.5f;
  Vector3 m_color = { 1, 1, 1 };*/

};

struct PointLight //: public AmbientLight
{
  PointLight()
  {}
  ~PointLight()
  {}

  Vector3 m_pos; float p1 = 0.f;

  Vector3 m_ambient; float p21 = 0.f;
  Vector3 m_diffuse; float p3 = 0.f;
  Vector3 m_specular; 

  float m_constant;
  float m_linear;
  float m_quadratic;

  int m_active;
  float padd;
};

struct DirectionalLight : public AmbientLight
{
  DirectionalLight()
  {
    /*for (int i = 0; i < SHADOWMAP_SIZE; ++i)
    {
      m_shadowMaps[i] = nullptr;
    }*/
  }

  ~DirectionalLight()
  {
    /*for (int i = 0; i < SHADOWMAP_SIZE; ++i)
    {
      if (m_shadowMaps[i] != nullptr)
        delete m_shadowMaps[i];
    }*/
  }
  const int SHADOWMAP_SIZE = 3;
  Vector3 m_pos;
  Vector3 m_dir = { -0.45f, -0.45f, -0.45f };
  Matrix4x4 m_CSMVPMats[3];
  //ShadowMapFBO* m_shadowMaps[3];
  //std::vector<MeshComponent*>

};