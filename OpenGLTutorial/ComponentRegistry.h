#pragma once
#include "Component.h"

class Component;


class ComponentRegistryBase
{

  ComponentID id_;
public:
  ComponentRegistryBase(ComponentID cid) :id_(cid){}
  virtual Component* Create() = 0;
  virtual ~ComponentRegistryBase() {}
};


template<typename T>
class ComponentRegistry : public ComponentRegistryBase
{
public:
  typedef T self_type;
  T self;
  bool m_editorUpdate;

  ComponentRegistry(ComponentID cid, bool editorUpdate) : ComponentRegistryBase(cid), m_editorUpdate(editorUpdate)
  {}

  virtual T* Create()
  {
    T* ret = new T();
    //->m_editorUpdate = m_editorUpdate;
    return ret;

  }
  ~ComponentRegistry()
  {
  }
};

template <typename T>
ComponentRegistryBase* AllocateForComponentRegistry(ComponentID cid, bool editorUpdate)
{
  return new ComponentRegistry<T>(cid, editorUpdate);
  //return New(ComponentRegistry<T>, cid, editorUpdate);
}