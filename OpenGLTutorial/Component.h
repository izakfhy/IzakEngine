#pragma once
#include <string>
#include "RefelectionUtils.h"

#define REGISTER_COMPONENT_AND_MGR(name,flag, duplicate, editorUpdate) #name, 
const std::string ComponentIDString[]
{
#include "ComponentNames.h"
};
#undef REGISTER_COMPONENT_AND_MGR

#define REGISTER_COMPONENT_AND_MGR(name,flag, duplicate, editorUpdate) name, 
enum class ComponentID
{
#include "ComponentNames.h"
  LAST
};
#undef REGISTER_COMPONENT_AND_MGR



class GameObject;

class Component
{
protected:
  bool m_markforDelete = false;
  bool m_active = true;
  std::string m_name = "";
  std::string m_uniquename = "";
  ComponentID m_id;

  GameObject* parent = nullptr;
public:
  std::string GetName() const;
  void SetName(const std::string& n);
  std::string GetUniqueName() const;
  void SetUniqueName(const std::string& n);

  void SetID(ComponentID id);
  ComponentID GetID() const;
  void MarkForDeletion();
  bool IsMarkForDeletion() const;

  bool IsActive() const;
  void SetActive(bool a);

  GameObject* GetParent();
  void SetParent(GameObject* obj);

  Component(ComponentID cid, const std::string& name, bool activ = true) :m_id(cid), m_name(name), m_active(activ), m_markforDelete(false){  }

  explicit Component(ComponentID cid, const std::string& name, std::string uniquename = "", bool activ = true) :m_id(cid),
    m_active(activ), m_name(name), m_uniquename(uniquename) {  }

  //virtual void Serialize(BBE::Serializer& ser) = 0;
  virtual void Init(){};
  virtual void Deinit(){};
  virtual void Update(){};
  virtual Component* Clone() = 0;
  virtual ~Component()
  {
    //parent = nullptr;

  }
  META_DATA(Component);
};
template <typename E>
typename std::underlying_type<E>::type to_underlying(E e) {
  return static_cast<typename std::underlying_type<E>::type>(e);
}
