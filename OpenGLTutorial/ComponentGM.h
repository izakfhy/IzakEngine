/* Start Header ***************************************************************/
/*!
\file   ComponentGM.h
\author Izak Foong, foong.h, 440002413
\par    foong.h\@digipen.edu
\date   

Project Name: 

\brief This is the declaration and definition of the component manager base
        class

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
#pragma once
#include <map>
#include "SystemComponentBase.h"
#include "ComponentManager.h"

enum class ComponentID;
class CMBase;


  class ComponentGM : public SystemComponentBase
  {

  public:
    static ComponentGM* SingletonPtr;
    std::map<ComponentID, CMBase*> gm;

    ComponentGM();
    static ComponentGM* GetInstance();

    void RegisterManager(ComponentID, CMBase* manager);
    void UpdateAllManagers();

    void ClearComponents();
    void ClearArchtypeVec();

    bool HasManager(ComponentID);
	void RemoveDeletedCmp();
    CMBase* GetManager(ComponentID);

    void SetEditMode(bool a);

    template <typename T>
    ComponentManager<T>* GetManager()
    {
      for (auto & elem : gm)
      {
        auto mgr = dynamic_cast<ComponentManager<T>*>(elem.second);
        if (mgr)
          return mgr;
      }
      std::string msg = std::string("no such manager: ") + typeid(T).name() + " !";
      std::cout << msg << std::endl;
      //ASSERTMSG(1, msg);
      return nullptr;
    }
  
    void BackupClone();
    void ResetFromBackup();

    ~ComponentGM();

    //virtuals
    virtual void Update();
    virtual void Init(){}
    virtual void Load(){}
    virtual void Free(){}
    virtual void Unload(){}
  };

  template <typename T>
  CMBase* AllocateManager()
  {
      return new ComponentManager<T>();;
  }
