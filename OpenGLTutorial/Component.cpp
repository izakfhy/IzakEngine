#include "Component.h"


DEFINE_META(Component)
{
  ADD_MEMBER(m_markforDelete,false);
  ADD_MEMBER(m_active, true);
  ADD_MEMBER(m_name, true);
  ADD_MEMBER(m_uniquename, true);
  ADD_MEMBER(m_id, false);
}

std::string  Component::GetName() const
{
  return m_name;
}

void  Component::SetName(const std::string& n)
{
  m_name = n;
}

bool  Component::IsActive() const
{
  return m_active;
}

void Component::SetActive(bool a)
{
  m_active = a;
}


GameObject* Component::GetParent()
{
  return parent;
}
void Component::SetParent(GameObject* obj)
{
  parent = obj;
}

std::string Component::GetUniqueName() const
{
  return m_uniquename;
}
void Component::SetUniqueName(const std::string& n)
{
  m_uniquename = n;
}

void Component::MarkForDeletion()
{
  m_markforDelete = true;
}
bool Component::IsMarkForDeletion() const
{
  return m_markforDelete;
}

ComponentID Component::GetID() const
{
  return m_id;
}

void Component::SetID(ComponentID id)
{
  m_id = id;
}