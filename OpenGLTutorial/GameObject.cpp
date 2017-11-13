#include "GameObject.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include "ComponentGM.h"
#include "Transform.h"



DEFINE_META(GameObject)
{
  ADD_MEMBER(m_name, true);
  ADD_MEMBER(m_components, false);
  ADD_MEMBER(m_id, true);
  ADD_MEMBER(m_parent, false);
  ADD_MEMBER(m_active, true);
  ADD_MEMBER(m_isDeleted, false);
  ADD_MEMBER(m_isArchtype, true);
  ADD_MEMBER(m_archtypename, true);
  ADD_MEMBER(m_transform, false);
}


GameObject::~GameObject()
{

}

GameObject::GameObject(bool isactive, bool isarchetype) :m_id(0), m_active(isactive), m_isArchtype(isarchetype), m_isDeleted(false)
{

}

GameObject::GameObject(const int newid, const bool isactive) : m_id(newid), m_active(isactive), m_isDeleted(false)
{

}

GameObject::GameObject() : m_id(0), m_active(false), m_isDeleted(false)
{

}

GameObject::GameObject(int i) : m_id(i), m_active(false), m_isDeleted(false)
{

}


void GameObject::AddComponent(Component* newcomp, bool addToArchType)
{
  //doing this for now unless we have component managers.
  // components_.push_back(newcomp);

  auto it = m_components.find(newcomp->GetUniqueName());

  if (it != m_components.end())
  {
    std::string msg("Components with unique name(");
    msg += newcomp->GetUniqueName();
    msg += ") already existed!";
    std::cout << msg << std::endl;
    //MessageBoxA(0, msg.c_str(), "", MB_OK);
    return;
  }
  m_components.insert({ newcomp->GetName(), newcomp });

  
  //code for working with component managers////////
  newcomp->SetParent(this);
  if (ComponentGM::GetInstance()->HasManager(newcomp->GetID()) == false)
  {
    std::cout << "no such manager registered! " << newcomp->GetName() << std::endl;
    std::abort();
  }
  /*
  auto col = dynamic_cast<CollisionComponent*>(newcomp);
  if (col && !addToArchType)
    g_collision->AddToVec(newcomp);*/
 
  if (!addToArchType)
    ComponentGM::GetInstance()->GetManager(newcomp->GetID())->AddToVec(newcomp);
  else
    ComponentGM::GetInstance()->GetManager(newcomp->GetID())->AddToArchVec(newcomp);
}


std::string GameObject::GetTypeName()const
{
  return m_archtypename;
}
void GameObject::SetTypeName(const std::string& newtype)
{
  m_archtypename = newtype;
}


std::map<std::string, Component*>& GameObject::GetComponentsMap()
{
  return m_components;
}

GameObject* GameObject::Clone()
{
  GameObject* clonedobj = new GameObject(m_active);
  for (auto &elem : m_components)
  {
    if (elem.second->GetUniqueName() != "GeneratedBS" && elem.second->GetUniqueName() != "GeneratedAABB")
      clonedobj->AddComponent(elem.second->Clone());
  }
  clonedobj->m_archtypename = m_archtypename;
 // clonedobj->m_group = m_group;
  return clonedobj;
}


GameObject* GameObject::CloneForArcheType()
{
  GameObject* clonedobj = new GameObject(m_active);
  for (auto &elem : m_components)
  {
    clonedobj->AddComponent(elem.second->Clone(), true);
  }
  clonedobj->m_archtypename = m_archtypename;
  return clonedobj;
}

void GameObject::SetId(const unsigned newid)
{
  m_id = newid;
}

void GameObject::SetDelete(const bool ndelete)
{
  m_isDeleted = ndelete;
}

bool GameObject::IsDeleted()const
{
  return m_isDeleted;
}

unsigned GameObject::GetID()const
{
  return m_id;
}

void GameObject::SetUniqueName(const std::string& name)
{
  std::stringstream ss;
  ss << name << "_" << m_id;
  m_name = ss.str();
}

std::string GameObject::GetUniqueName() const
{
  return m_name;
}

void GameObject::SetComponentActive(std::string name, const bool newactive)
{
  m_components[name]->SetActive(newactive);


  std::cout << "Component name(" + name + ") not found!" << std::endl;
}

void GameObject::InitializeComponents()
{
  for (auto elem : m_components)
  {
    elem.second->Init();
  }
}

void GameObject::SetComponentActive(ComponentID cid, const bool newactive)
{
  m_components[ComponentIDString[to_underlying(cid)]]->SetActive(newactive);
}

void GameObject::SetActive(const bool newactive)
{
  m_active = newactive;
  SetAllActive(m_active);
}

void GameObject::SetAllActive(const bool newactive)
{
  for (auto& elem : m_components)
    elem.second->SetActive(newactive);
}

bool GameObject::IsActive()const
{
  return m_active;
}

void GameObject::MarkForDelete()
{
  SetActive(false);
  RemoveAllComponents();
  //CMBase::cmpDeleted = true;
  //SetDelete(true);
  //g_factory->m_deleteMarked = true;
}

bool GameObject::HasComponent(const std::string &name)
{
  return m_components.find(name) != m_components.end();

}
Transform* GameObject::GetTransform()
{ 
  return GetComponent<Transform>(); 
}
GameObject* GameObject::GetParent()
{
  return m_parent;
}

void GameObject::RemoveComponent(ComponentID cid)
{
  auto elem = m_components[ComponentIDString[to_underlying(cid)]];
  elem->Deinit();
  elem->SetActive(false);
  m_components.erase(ComponentIDString[to_underlying(cid)]);
  
}

void GameObject::RemoveComponent(const std::string& name)
{
  auto elem = m_components[name];
  elem->Deinit();
  elem->SetActive(false);
  elem->MarkForDeletion();
  m_components.erase(name);
 
}

void GameObject::RemoveAllComponents()
{
  for (auto& elem : m_components)
  {
    elem.second->Deinit();
    elem.second->MarkForDeletion();
  }

  m_components.clear();
}

void GameObject::SerializeObject(std::ostream& os)
{
  os << m_archtypename << std::endl;
  os << "{" << std::endl;

  TextSerializer::AddPad();
  //Padding(os);
  refvar self = *this;
  self.Serialize(os);
  for (auto & elem : m_components)
  {
    refvar temp = *(elem.second);
    temp.SetMeta(MetaManager::Get(elem.first));
    temp.Serialize(os);
  }
  TextSerializer::AddPad();
  os << "}" << std::endl;
  TextSerializer::DecrementPad();
  TextSerializer::DecrementPad();
}

void GameObject::DeserializeObject(std::istream& is)
{
  is >> m_archtypename;
  SkipLine(is);

}
void GameObject::SerializeArchetype(std::ostream& os)
{
  SerializeObject(os);
}