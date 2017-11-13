#pragma once
#include <string>
#include <vector>
#include <map>
#include "Component.h"
#include "RefelectionUtils.h"

class Transform;

class GameObject
{
  std::string m_name = "";
  std::map<std::string,Component*> m_components;
  unsigned m_id;
  GameObject* m_parent;
  bool m_active = true;
  bool m_isDeleted = false;
  bool m_isArchtype = false;
  std::string m_archtypename = "";
  Transform* m_transform = nullptr;

public:
    
  virtual ~GameObject();
  explicit GameObject(bool isactive, bool isarchetype);
  GameObject(const int newid, const bool isactive);
  GameObject();
  explicit GameObject(int i);

  //settors
  void SetTypeName(const std::string& newtype);
  void SetComponentActive(ComponentID cid, const bool newactive);
  void SetComponentActive(std::string name, const bool newactive);
  void SetAllActive(const bool newactive);
  void SetId(const unsigned newid);
  void SetOriginalActive(const bool newa);
  void SetActive(const bool newactive);
  void SetDelete(const bool ndelete);
  void SetUniqueName(const std::string& name);


  //getttors
  std::string   GetTypeName()const;
  bool          IsDeleted()const;
  unsigned      GetID()const;
  std::string   GetUniqueName() const;
  bool          IsActive()const;
  bool          HasComponent(const std::string &name);
  GameObject*   GetParent();
  Transform*    GetTransform();
  std::map<std::string, Component*>& GetComponentsMap();

  void RemoveAllComponents();
  void AddComponent(Component* newcomp, bool addToArchType = false);
  void MarkForDelete();

  void SerializeObject(std::ostream& os);
  void DeserializeObject(std::istream& is);
  void SerializeArchetype(std::ostream& os);
  void InitializeComponents();

  void Update()
  {
    for (auto& elem : m_components)
      elem.second->Update();
  }

  GameObject* Clone();
  GameObject* CloneForArcheType();

  void RemoveComponent(ComponentID cid);
  void RemoveComponent(const std::string& name);

  template <typename T>
  T* GetComponent()
  {
    for (auto elem : m_components)
    {
      auto cmp = dynamic_cast<T*>(elem.second);
      if (cmp)
        return cmp;
    }
    return nullptr;
  }

  template <typename T>
  T* GetComponent(std::string uniquename)
  {
    for (auto& elem : m_components)
    {
      if (elem.second->m_uniquename == uniquename)
      {
        auto cmp = dynamic_cast<T*>((*elem.second));
        if (cmp)
          return cmp;
      }
    }
  
    //ERROR_MSG("No Such Component(" + uniquename + ") found in game object: " + archename);
    return nullptr;
  }

  template <typename T>
  std::vector<T*> GetSameComponents()
  {
    std::vector<T*> samecomponents;
    for (auto & elem : components_)
    {
      auto temp = dynamic_cast<T*>(elem.second);
      if (temp)
      {
        samecomponents.push_back(temp);
      }

    }
    return samecomponents;
  }
  META_DATA(GameObject);
};