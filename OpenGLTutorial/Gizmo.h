#pragma once
#include "GameObject.h"
#include <memory>
#include "Shader.h"

class ModelComponent;

enum class GizmoType
{
  Translate = 0,
  Scale,
  Rotate
};

class Gizmo
{

  GizmoType m_type;
  GameObject* m_target = nullptr;
  Transform* m_trf = nullptr;
public:

  union
  {
    struct
    {
      GameObject* m_left;
      GameObject* m_front;
      GameObject* m_up;
    };
    GameObject* m_gizmos[3];
  };


  Gizmo();
  ~Gizmo();
  void Init();
  void Update();
  void DeInit();

  void SetShader(std::shared_ptr<Shader> ptr);
  void SetTarget(GameObject* t);
  GameObject* GetTarget();

};