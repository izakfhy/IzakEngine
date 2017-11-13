#pragma once

/* Start Header ***************************************************************/
/*!
\file   Factory.h
\author Izak Foong, foong.h, 440002413
\par    foong.h\@digipen.edu
\date   April 11, 2015

Project Name: Bollywood Hero

\brief This is the declaration and factory class

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
#pragma once
#include <map>
#include "GameObject.h"
#include "ComponentRegistry.h"
#include "BBEMath.h"
#include "SystemComponentBase.h"


typedef std::string ArchtypeName;

class XMLSerializer;

  class Factory : public SystemComponentBase
  {
  public:
    static unsigned int GameObjId;
    static bool m_deleteMarked;
    unsigned int InitialObjects;

    std::map<unsigned, GameObject*> GameObjectsMap;
    std::map<unsigned, GameObject*> BackupGameObjectsMap;
    std::map<std::string, ComponentRegistryBase*> Componentregistry;
    std::map<ArchtypeName, GameObject*> Archetypemap;
    std::map<ArchtypeName, std::vector<GameObject*>> sameTypeVec;
    std::map<ArchtypeName, std::vector<GameObject*>> backupSameTypeVec;
    std::string GetLevelName(const std::string&);

    bool ArchetypeExists(const std::string& name);
    bool loadArchtypeComponents(std::istream& ser, GameObject* gameobj, const std::string& cmpname);
    void ClearRegistry();
    void ClearArchetypes();


    static Factory* SingletonPtr;
    Factory();
    ~Factory();
    std::map<unsigned, GameObject*>& GetObjMap();
    void RegisterComponent(std::string name, ComponentRegistryBase*);

    void SetObjectInactive(size_t id);
    void SetInitialObjAmount(const unsigned int val);

    void InitializeObjs(unsigned int start, unsigned int end);
    bool LoadArchetypes(const std::string& fname);
    bool LoadAllArchetypes(const std::string& directory);
    bool LoadSceneFromFile(const std::string& fname, bool init = true);
    bool LoadAddonFromFile(const std::string& fname, bool init = true, unsigned int* startID = nullptr, unsigned int* endID = nullptr);
    bool LoadContentsFromFile(XMLSerializer& ser, bool init, unsigned int* startID = nullptr, unsigned int* endID = nullptr);
    bool LoadAvailableObjFile();
    void MakeBackup();
    void RestoreFromBackup();
    void DeleteMarkedObj();

    static Factory*									                    GetInstance();
    GameObject*										                      GetObjectByID(unsigned int gid);
    std::vector<GameObject*>&					                  GetObjectListByName(const std::string& type_name);
    GameObject*                                         GetObjectByName(const std::string & uniqueName);
    std::map<std::string, GameObject*>&				          GetArchtypeMap();
    std::map<std::string, ComponentRegistryBase*>& 	    GetRegistryMap();
    std::map<std::string, std::vector<GameObject*>>&      GetObjectListMap();
    unsigned int GetInitialObjAmount()const;

    bool SaveSceneToFile(const std::string& fname);
    bool SaveArchetypes(const std::string& fname);
    bool SaveArchetypes(const std::string& fname, GameObject* obj);

    GameObject* CreateEmptyObject();
    GameObject* CloneFromArchType(const std::string name);
    GameObject* CloneFromGameObject(GameObject*);

    //void SendMsgToObjects(const std::string&, MessageObject&);
    //id functions

    static unsigned int GetNumberOfObj();

    void ClearObjectPool();
    void DeleteGameObject(unsigned);

    //virtuals
    virtual void Load();
    virtual void Init() override;
  };

#define REGISTER_COMPONENT(name) BBE::Factory::GetInstance()->RegisterComponent(#name, AllocateForComponentRegistry<name>(ComponentID::name));
#define REGISTER_MGR(type,obj) ComponentGM::GetInstance()->RegisterManager(type, obj);


//#define REGISTER_COMPONENT_AND_MGR(name) BBE::Factory::GetInstance()->RegisterComponent(#name, AllocateForComponentRegistry<name>(type)),\
//	ComponentGM::GetInstance()->RegisterManager(type,AllocateManager<name>());// ComponentManager<name>::GetInstance());
//
//#include "ComponentNames.h"
//
//#undef REGISTER_COMPONENT_AND_MGR

