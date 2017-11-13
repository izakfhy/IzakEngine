#include "PointLightComponent.h"


DEFINE_META(PointLightComponent)
{
  PARENT_META(Component);
  ADD_MEMBER(m_ambient, true);
  ADD_MEMBER(m_diffuse, true);
  ADD_MEMBER(m_specular, true);
  ADD_MEMBER(m_color, true);
  ADD_MEMBER(m_constant, true);
  ADD_MEMBER(m_linear, true);
  ADD_MEMBER(m_quadratic, true);
}


PointLightComponent::PointLightComponent() : Component(ComponentID::PointLightComponent, "PointLightComponent", "PointLightComponent", true)
{}
PointLightComponent::PointLightComponent(Vector3 am, Vector3 dif, Vector3 spec, float col, float con, float lin, float quad) :
Component(ComponentID::Transform, "PointLightComponent", "PointLightComponent", true),
m_ambient(am),
m_diffuse(dif),
m_specular(spec),
m_color(col),
m_constant(con),
m_linear(lin),
m_quadratic(quad)
{}

//gettors
Vector3 PointLightComponent::Ambient() const
{
  return m_ambient;
}
Vector3 PointLightComponent::Diffuse() const
{
  return m_diffuse;
}
Vector3 PointLightComponent::Specular() const
{
  return m_specular;
}


float PointLightComponent::Constant() const
{
  return m_constant;
}
float PointLightComponent::Color() const
{
  return m_color;
}
float PointLightComponent::Linear() const
{
  return m_linear;
}
float PointLightComponent::Quadratic() const
{
  return m_quadratic;
}

//settors
void PointLightComponent::SetConstant(float c)
{
  m_constant = c;
}

void PointLightComponent::SetAmbient(const Vector3& a)
{
  m_ambient = a;
}
void PointLightComponent::SetDiffuse(const Vector3& d)
{
  m_diffuse = d;
}
void PointLightComponent::SetSpecular(const Vector3& s)
{
  m_specular = s; 
}
void PointLightComponent::SetColor(float c)
{
  m_color = c;
}
void PointLightComponent::SetLinear(float l)
{
  m_linear = l;
}
void PointLightComponent::SetQuadratic(float q)
{
  m_quadratic = q;
}

//virtuals
void PointLightComponent::Init()
{

}
void PointLightComponent::Deinit()
{

}
void PointLightComponent::Update()
{
}
Component* PointLightComponent::Clone(void) 
{
  return new PointLightComponent(*this);
}
