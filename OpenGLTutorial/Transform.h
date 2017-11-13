#pragma once

#include "BBEMath.h"
#include "Component.h"

class Transform : public Component
{
public:
  Transform(void);
  //  Transform            (const Transform&);
  ~Transform(void);


  void TransformUpdate(void);
  void SetScale(const Vector3&);
  void SetPosition(const Vector3&);
  void SetRotation(const Vector3&);
  void LookAt(const Vector3&);
  void AddRotation(float degrees, const Vector3& axis);
  void ScaleChild();
  Vector3 GetScale(void) const { return m_Scale; }
  Vector3 GetPosition(void) const { return m_Pos; }
  Vector3 GetWorldPosition(void) const;
  Vector3 GetRotation(void) const;
  Matrix4x4 GetModelMat(void) const { return m_Modelmat; }

  //void Serialize(BBE::Serializer& ser)  override final;
  //void LoadLevelData(BBE::Serializer& ser)  override final;
  //void WriteLevelData(BBE::Serializer& ser)  override final;
  //void Deserialize(BBE::Serializer& ser)  override final;
  //void Receive(MessageObject* msg)    override final;
  //void SetTWOptions(const std::string& bar)override final;
  void Init() override final;
  void Deinit() override final;
  void Update() override final;
  Component* Clone(void) override final;

  Vector3    m_Scale = { 1, 1, 1 };
  Vector3    m_Pos;
  Vector3    m_Rotate;
  Vector3    m_FrontVec = { 0, 0, 1 };
  Vector3    m_UpVec = { 0, 1, 0 };
  Vector3    m_RightVec = { 1, 0, 0 };
  Vector3    m_ViewFramePos;
  Matrix4x4  m_Modelmat;
  Matrix4x4  m_PrevModelMat;
  Quaternion m_RotQuaternion = { 0, 0, 0, 1 };

  META_DATA(Transform);
private:
  static void __stdcall SetRotateCallback(const void *value, void *clientData);
  static void __stdcall GetRotateCallback(void *value, void *clientData);
};
