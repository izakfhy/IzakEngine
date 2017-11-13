#include "Transform.h"
#include "GameObject.h"

DEFINE_META(Transform)
{
  PARENT_META(Component);
  ADD_MEMBER(m_Scale, true);
  ADD_MEMBER(m_Pos, true);
  ADD_MEMBER(m_Rotate, true);
  ADD_MEMBER(m_FrontVec, false);
  ADD_MEMBER(m_UpVec, false);
  ADD_MEMBER(m_ViewFramePos, false);
  ADD_MEMBER(m_Modelmat, false);
  ADD_MEMBER(m_PrevModelMat, false);
  ADD_MEMBER(m_RotQuaternion, false);

}

Transform::Transform()
  : Component(ComponentID::Transform, "Transform", "Transform", true)
{}

Transform::~Transform()
{

}

void Transform::Update()
{
 // if (!parent || !parent->GetParent())
    TransformUpdate();
}

void Transform::Init()
{

}
void Transform::Deinit()
{

}

void Transform::TransformUpdate(void)
{
  Matrix4x4 Scale, Translate;

  Scale.MakeScaleMatrix(m_Scale.x, m_Scale.y, m_Scale.z);
  Translate.MakeTranslationMatrix(m_Pos.x, m_Pos.y, m_Pos.z);

  Matrix4x4 RotMat = m_RotQuaternion.GetMatrix();

  m_Modelmat = Translate * RotMat * Scale;

  const float VectorLengthError = 1.1f;
  if (m_FrontVec.LengthSq() > VectorLengthError) m_FrontVec.Normalize();
  if (m_RightVec.LengthSq() > VectorLengthError) m_FrontVec.Normalize();
  if (m_UpVec.LengthSq()    > VectorLengthError) m_FrontVec.Normalize();

  if (parent)
  {
    auto objParent = parent->GetParent();
    if (objParent)
    {
      m_Modelmat = objParent->GetTransform()->GetModelMat() * m_Modelmat;
    }

    /*for (auto& childIter : parent->GetChilds())
    {
      childIter->GetTransform()->TransformUpdate();
    }*/
  }
}

void Transform::SetScale(const Vector3& s)
{
  m_Scale = s;
}

void Transform::SetPosition(const Vector3& p)
{
  m_Pos = p;
}

void Transform::SetRotation(const Vector3& r)
{
  Quaternion RotX = { r.x, { 1, 0, 0 } };
  Quaternion RotY = { r.y, { 0, 1, 0 } };
  Quaternion RotZ = { r.z, { 0, 0, 1 } };
  m_RotQuaternion = RotX * RotY * RotZ;

  // rotate front and right vectors
  m_FrontVec = m_RotQuaternion.GetMatrix() * Vector4(m_FrontVec, 0);
  m_RightVec = m_RotQuaternion.GetMatrix() * Vector4(m_RightVec, 0);
  m_UpVec = m_FrontVec.Cross(m_RightVec);
}

void Transform::LookAt(const Vector3& v)
{
  Vector3 TargetToParentVec = v - m_Pos;
  Vector3 TargetToParentVecNormalized = TargetToParentVec.Normalize();

  Vector3 RotationAxis = m_FrontVec.Cross(TargetToParentVecNormalized);
  float RotationAngle;

  // special case for 180 degree turn
  if (RotationAxis.LengthSq() == 0)
  {
    RotationAxis = m_UpVec;
    RotationAngle = TargetToParentVecNormalized.Dot(m_FrontVec) > 0.f ? 0.f : 180.f;
  }
  else
  {
    RotationAngle = ToDegree(asin(RotationAxis.Length()));
    RotationAxis.NormalizeThis();
  }

  // rotate front and right vectors
  Quaternion q = { RotationAngle, RotationAxis };
  m_RotQuaternion = q * m_RotQuaternion;
  Matrix4x4 RotationMatrix = q.GetMatrix();
  m_FrontVec = RotationMatrix * Vector4(m_FrontVec, 0);
  m_RightVec = RotationMatrix * Vector4(m_RightVec, 0);
  m_UpVec = m_FrontVec.Cross(m_RightVec);
}

void Transform::AddRotation(float degrees, const Vector3 & axis)
{
  Quaternion q(degrees, axis);
  m_RotQuaternion = q * m_RotQuaternion;
  m_RotQuaternion.NormalizeThis();

  // rotate front and right vectors
  Matrix4x4 RotationMatrix = q.GetMatrix();
  m_FrontVec = RotationMatrix * Vector4(m_FrontVec, 0);
  m_RightVec = RotationMatrix * Vector4(m_RightVec, 0);
  m_UpVec = m_FrontVec.Cross(m_RightVec);
}

void Transform::ScaleChild()
{
  auto parObj = parent->GetParent();
  if (parObj)
  {
    Vector3 parScale = parObj->GetTransform()->GetScale();
    m_Scale.x /= parScale.x;
    m_Scale.y /= parScale.y;
    m_Scale.z /= parScale.z;
  }

}

Vector3 Transform::GetRotation() const
{
  return m_RotQuaternion.GetEulerAnglesDegrees();
}
/*
void Transform::Serialize(BBE::Serializer& ser)
{
  BBE::XMLSerializer& xmlser = dynamic_cast<BBE::XMLSerializer&>(ser);
  xmlser.AttachAttribute("x", m_Pos.x);
  xmlser.AttachAttribute("y", m_Pos.y);
  xmlser.AttachAttribute("z", m_Pos.z);
  xmlser.AttachAttribute("scalex", m_Scale.x);
  xmlser.AttachAttribute("scaley", m_Scale.y);
  xmlser.AttachAttribute("scalez", m_Scale.z);
  xmlser.AttachAttribute("QuaternionX", m_RotQuaternion.x);
  xmlser.AttachAttribute("QuaternionY", m_RotQuaternion.y);
  xmlser.AttachAttribute("QuaternionZ", m_RotQuaternion.z);
  xmlser.AttachAttribute("QuaternionW", m_RotQuaternion.w);
}

void Transform::Deserialize(BBE::Serializer& ser)
{
  BBE::XMLSerializer& xmlser = dynamic_cast<BBE::XMLSerializer&>(ser);
  m_uniquename = xmlser.GetAttributeFromCurrentS("name", std::string("Transform"));
  m_Pos.x = xmlser.GetAttributeFromCurrent("x", 0.f);
  m_Pos.y = xmlser.GetAttributeFromCurrent("y", 0.f);
  m_Pos.z = xmlser.GetAttributeFromCurrent("z", 0.f);
  m_Scale.x = xmlser.GetAttributeFromCurrent("scalex", 0.1f);
  m_Scale.y = xmlser.GetAttributeFromCurrent("scaley", 0.1f);
  m_Scale.z = xmlser.GetAttributeFromCurrent("scalez", 0.1f);
  m_RotQuaternion.x = xmlser.GetAttributeFromCurrent("QuaternionX", 0.0f);
  m_RotQuaternion.y = xmlser.GetAttributeFromCurrent("QuaternionY", 0.0f);
  m_RotQuaternion.z = xmlser.GetAttributeFromCurrent("QuaternionZ", 0.0f);
  m_RotQuaternion.w = xmlser.GetAttributeFromCurrent("QuaternionW", 1.0f);

  Matrix4x4 RotMat = m_RotQuaternion.GetMatrix();
  m_FrontVec = RotMat * Vector4({ 0, 0, 1 }, 0);
  m_RightVec = RotMat * Vector4({ 1, 0, 0 }, 0);
  m_UpVec = m_FrontVec.Cross(m_RightVec);
}

void Transform::LoadLevelData(BBE::Serializer& ser)
{
  Deserialize(ser);
}

void Transform::WriteLevelData(BBE::Serializer& ser)
{
  Serialize(ser);
}

void Transform::Receive(MessageObject* msg)
{

}*/

Component* Transform::Clone()
{
  return new Transform(*this);
}

void __stdcall Transform::SetRotateCallback(const void * value, void * clientData)
{
  static_cast<Transform*>(clientData)->SetRotation(*static_cast<const Vector3*>(value));
}

void __stdcall Transform::GetRotateCallback(void * value, void * clientData)
{
  *static_cast<Vector3 *>(value) = static_cast<Transform*>(clientData)->m_RotQuaternion.GetEulerAnglesDegrees();
}

Vector3 Transform::GetWorldPosition(void) const
{
  Vector4 ret = Vector4(m_Modelmat.m[0][3], m_Modelmat.m[1][3], m_Modelmat.m[2][3], 1);
  /*if (parent)
  {
  auto objParent = parent->GetParent();
  if (objParent)
  {
  ret = objParent->GetTransform()->GetModelMat() * ret;
  }
  }*/
  return ret;
}