#include "Gizmo.h"
#include "Model.h"
#include "Transform.h"
#include "Globals.h"
#include "Factory.h"

#include "GameObject.h"

Gizmo::Gizmo() :
m_type(GizmoType::Translate),
m_left(nullptr),
m_front(nullptr),
m_up(nullptr)
{

}

Gizmo::~Gizmo()
{

}


void Gizmo::SetTarget(GameObject* t)
{
  m_target = t;
}

void Gizmo::SetShader(std::shared_ptr<Shader> ptr)
{
  m_up->GetComponent<ModelComponent>()->SetShader(ptr);
  m_front->GetComponent<ModelComponent>()->SetShader(ptr);
  m_left->GetComponent<ModelComponent>()->SetShader(ptr);
}

GameObject* Gizmo::GetTarget()
{
  return m_target;
}

void Gizmo::Init()
{
  /*m_left = new ModelComponent();
  m_left->LoadMesh("./Resource/Models/Gizmo/TranslateGizmo.obj");
  m_front = new ModelComponent();
  m_front->LoadMesh("./Resource/Models/Gizmo/TranslateGizmo.obj");
  m_up = new ModelComponent();
  m_up->LoadMesh("./Resource/Models/Gizmo/TranslateGizmo.obj");*/

  m_left = g_factory->CloneFromArchType("Gizmo");
  m_left->InitializeComponents();
  m_front = g_factory->CloneFromArchType("Gizmo");
  m_front->GetTransform()->SetRotation({ 0, 90, 0 });
  m_front->InitializeComponents();
  m_up = g_factory->CloneFromArchType("Gizmo");
  m_up->GetTransform()->SetRotation({ 0, 0, 90 });
  m_up->InitializeComponents();
  m_trf = m_left->GetTransform();
}

void Gizmo::Update()
{
  if (m_target)
  {
    Transform* trf = m_target->GetComponent<Transform>();
    if (trf)
    {
      m_trf->m_Pos = trf->m_Pos;
      for (auto elem : m_gizmos)
      {
        elem->GetComponent<Transform>()->m_Pos = trf->m_Pos;
      }

    }
  }
}

void Gizmo::DeInit()
{

}