/* Start Header ***************************************************************/
/*!
\file   ComponentGM.cpp
\author Izak Foong, foong.h, 440002413
\par    foong.h\@digipen.edu
\date   December 4, 2015

Project Name: Trojan XL

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "ComponentGM.h"
#include "ComponentManager.h"
#include "Component.h"
#include "ResourceManager.h"


#define MAX_SIZE 10000


  ComponentGM* ComponentGM::SingletonPtr = nullptr;

  ComponentGM::ComponentGM()
  {
    SingletonPtr = this;
  }
  
  void ComponentGM::RegisterManager(ComponentID cid, CMBase* manager)
  {
  //  manager->gvec.reserve(MAX_SIZE);
    manager->m_id = cid;
    gm[cid] = manager;
  }

  void ComponentGM::UpdateAllManagers()
  {
    //if (CMBase::cmpDeleted)
    //{
    //  RemoveDeletedCmp();
    //  CMBase::cmpDeleted = false;
    //}
    for (auto& elem : gm)
    {
      std::string ComponentIDName = ComponentIDString[(int)elem.first];
      //Profiling(ComponentIDName);
      if (elem.second->IsActive())
      {
        elem.second->Update();
      }
    }
  }

  ComponentGM* ComponentGM::GetInstance()
  {
    return SingletonPtr;
  }

  bool ComponentGM::HasManager(ComponentID cid)
  {
    std::map<ComponentID, CMBase*>::iterator it = gm.find(cid);
    if (it == gm.end())
      return false;
    return true;
  }

  CMBase* ComponentGM::GetManager(ComponentID cid)
  {
    return gm[cid];
  }

  void ComponentGM::SetEditMode(bool a)
  {
    //GetManager(ComponentID::CollisionComponent)->SetManagerActive(a);
    //GetManager(ComponentID::PhysicsComponent)->SetManagerActive(a);
  }

  void ComponentGM::RemoveDeletedCmp()
  {
  
      for (auto& elem : gm)
      {
        //if (elem.first == ComponentID::CollisionComponent)
          //continue;
        elem.second->RemoveDeletedCmp();
      }


  }

  void ComponentGM::BackupClone()
  {
    for (auto& elem : gm)
    {
      elem.second->CloneComponents();
    }
  }

  void ComponentGM::ResetFromBackup()
  {
    for (auto& elem : gm)
    {
      elem.second->ResetFromClone();
    }
  }

  ComponentGM::~ComponentGM()
  {
      
    for (auto& elem : gm)
    {
        elem.second->ClearArchVec();
        elem.second->ClearVec();
        delete elem.second;
        //Delete(elem.second);
    }
    gm.clear();
  }

  void ComponentGM::ClearComponents()
  {
    for (auto& elem : gm)
    {
      auto m = IZE::ResourceManager::GetInstance()->GetResourceMapContainer();
      elem.second->ClearVec();
	 
    }
  }

  void ComponentGM::ClearArchtypeVec()
  {
    for (auto& elem : gm)
    {
      elem.second->ClearArchVec();
    }
  }

  void ComponentGM::Update()
  {
    UpdateAllManagers();
 
  }
