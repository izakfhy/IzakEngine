#pragma once
#include "Component.h"
#include "BBEMath.h"
#include "RefelectionUtils.h"

class PointLightComponent : public Component
{
  Vector3 m_ambient;
  Vector3 m_diffuse;
  Vector3 m_specular;
  float m_color;
  float m_constant = 0;
  float m_linear;
  float m_quadratic;

public:
  PointLightComponent();
  PointLightComponent(Vector3 am, Vector3 dif, Vector3 spec, float col,float con, float lin, float quad);

  //gettors
  Vector3 Ambient() const;
  Vector3 Diffuse() const;
  Vector3 Specular() const;
  float Color() const;
  float Constant() const;
  float Linear() const;
  float Quadratic() const;

  //settors
  void SetAmbient(const Vector3& a);
  void SetDiffuse(const Vector3& d);
  void SetSpecular(const Vector3& s); 
  void SetColor(float c);
  void SetLinear(float l);
  void SetQuadratic(float q);
  void SetConstant(float c);
  //virtuals
  void Init() override final;
  void Deinit() override final;
  void Update() override final;
  Component* Clone(void) override final;

  META_DATA(PointLightComponent);
};