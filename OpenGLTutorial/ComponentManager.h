#pragma once
#include <vector>
#include <algorithm>
#include "Component.h"


class GameObject;
class Component;

class CMBase
{
  bool active;

public:
  //vars
  static bool cmpDeleted;
  ComponentID m_id;
  CMBase() :active(true){}

  void SetManagerActive(const bool a);
  bool IsActive()const;

  virtual void ClearArchVec() = 0;

  virtual void AddToVec(Component* newcmp) = 0;
  virtual void AddToArchVec(Component* newcmp) = 0;
  virtual void RemoveFromVec(Component*& cmp) = 0;

  virtual void ClearVec() = 0;
  virtual ~CMBase();
  virtual void Update() = 0;
  virtual void RemoveDeletedCmp() = 0;
  virtual void CloneComponents() = 0;
  virtual void ResetFromClone() = 0;
};


template<typename T>
class ComponentManager : public CMBase
{

public:

  std::vector<T*> m_vec;
  std::vector<T*> m_clonevec;
  std::vector<T*> m_archvec;
  static ComponentManager<T>* SingletonPtr;
  ComponentManager()
  {
    SingletonPtr = this;
    m_vec.reserve(100000);
  }

  virtual void RemoveFromVec(Component*& cmp)
  {
    for (unsigned i = 0; i < m_vec.size(); ++i)
    {
      if (m_vec[i] == cmp)
      {
        m_vec.erase(m_vec.begin() + i);
        break;
      }
    }

  }

  virtual void CloneComponents()
  {
    m_clonevec = std::move(m_vec);
  }

  virtual void ResetFromClone()
  {
    //  gvec.clear();
    m_vec = std::move(m_clonevec);

  }

  void AddToVec(Component* newcmp)
  {
    auto temp = dynamic_cast<T*>(newcmp);
    m_vec.push_back(temp);
  }

  void AddToArchVec(Component* newcmp)
  {
    auto temp = dynamic_cast<T*>(newcmp);
    m_archvec.push_back(temp);
  }
  void ClearVec()
  {
    for (auto& elem : m_vec)
    {
      delete elem;
      //SAFE_DELETE(elem);
    }
    m_vec.clear();
  }

  void ClearArchVec()
  {
    for (auto& elem : m_archvec)
    {
      delete elem;
    }

    m_archvec.clear();
  }

  static ComponentManager<T>* GetInstance()
  {
    return SingletonPtr;
  }

  virtual ~ComponentManager()
  {
    for (auto& elem : m_clonevec)
    {
      delete elem;
    }
    m_clonevec.clear();
    ClearVec();
    ClearArchVec();
  }

  virtual void RemoveDeletedCmp()
  {
    if (CMBase::cmpDeleted)
    {
      for (unsigned i = 0; i < m_vec.size(); ++i)
      {
        if (m_vec[i]->IsMarkForDeletion())
        {
          auto temp = m_vec.begin() + i;
          /*if (m_id != ComponentID::SphereCollider &&
            m_id != ComponentID::CubeCollider &&
            m_id != ComponentID::OCubeCollider)
            SAFE_DELETE(*temp);*/
          delete *temp;
          m_vec.erase(m_vec.begin() + i);
          --i;
        }
      }
    }

  }
  virtual void Update()
  {

    for (auto& elem : m_vec)
    {
      //if (elem->active == false || (g_editor->m_toggleEditMode && elem->m_editorUpdate == false))
      if (!elem->IsActive())
        continue;

      elem->Update();
    }
  }
};

template <typename T>
ComponentManager<T>* ComponentManager<T>::SingletonPtr = nullptr;
