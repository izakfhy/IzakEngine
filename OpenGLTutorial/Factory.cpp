#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include <iostream>
#include "Factory.h"
#include "XMLSerializer.h"
#include "SystemComponentHeaders.h"

#include "AllComponents.h"

Factory* Factory::SingletonPtr = nullptr;
bool Factory::m_deleteMarked = false;
unsigned int Factory::GameObjId = 1;
Factory::Factory() :InitialObjects(0)
{
  SingletonPtr = this;
}

Factory::~Factory()
{
  ClearRegistry();
  ClearArchetypes();
  ClearObjectPool();

  // clear back up
  RestoreFromBackup();
  ClearObjectPool();


}

void Factory::ClearRegistry()
{
  for (auto& elem : Componentregistry)
  {
    delete elem.second;
  }
  Componentregistry.clear();
}

void Factory::ClearArchetypes()
{
  for (auto& elem : Archetypemap)
  {
    delete elem.second;
  }
  Archetypemap.clear();

}

std::map<std::string, GameObject*>& Factory::GetArchtypeMap()
{
  return Archetypemap;
}

std::map<std::string, ComponentRegistryBase*>& Factory::GetRegistryMap()
{
  return Componentregistry;
}

unsigned int Factory::GetInitialObjAmount()const
{
  return InitialObjects;
}

void Factory::SetInitialObjAmount(const unsigned int val)
{
  InitialObjects = val;
}

unsigned int Factory::GetNumberOfObj()
{
  return GameObjId;
}

GameObject* Factory::GetObjectByID(unsigned int gid)
{
  return GameObjectsMap[gid];
}

GameObject * Factory::GetObjectByName(const std::string & uniqueName)
{
  for (auto& iter : GameObjectsMap)
  {
    if (iter.second->GetUniqueName() == uniqueName)
    {
      return iter.second;
    }
  }

  return nullptr;
}

Factory* Factory::GetInstance()
{
  return SingletonPtr;
}

GameObject* Factory::CreateEmptyObject()
{
  GameObject* gameobj = new GameObject();//New(GameObject);
  gameobj->SetId(GameObjId);
  GameObjectsMap[GameObjId++] = gameobj;
  return gameobj;
}

void Factory::SetObjectInactive(size_t id)
{
  GameObjectsMap[id]->SetActive(false);
}

GameObject* Factory::CloneFromArchType(const std::string name)
{
  assert(this);
  //ASSERTMSG(this, "CloneFromArchType() called before Factory is initialized!");

  try
  {
    return CloneFromGameObject(Archetypemap.at(name));
  }
  catch (...)
  {
    std::cout << "Archtype: " + name + " does not exists!" << std::endl;
   // ASSERTMSG(1, std::string("Archtype: " + name + " does not exists!").c_str());
  }

  return nullptr;
}

GameObject* Factory::CloneFromGameObject(GameObject* curObj)
{
  GameObject* newobj = curObj->Clone();
  assert(newobj);
  //ASSERTMSG(newobj, "Clone() function returned a nullptr");
  newobj->SetActive(true);
  //newobj->SetUpBS(false);
  //newobj->SetUpAABB(false);
  newobj->SetId(GameObjId);
  //newobj->InitComponents();
  GameObjectsMap[GameObjId++] = newobj;

  if (sameTypeVec.find(newobj->GetTypeName()) == sameTypeVec.end())
    sameTypeVec.insert({ newobj->GetTypeName(), std::vector<GameObject*>() });

  sameTypeVec[newobj->GetTypeName()].push_back(newobj);
  
  //g_collision->AddToOctree(newobj);
  return newobj;
}

void Factory::RegisterComponent(std::string name, ComponentRegistryBase *newcmp)
{
  Componentregistry[name] = newcmp;
}

std::map<unsigned, GameObject*>& Factory::GetObjMap()
{
  return GameObjectsMap;
}

bool Factory::LoadAllArchetypes(const std::string& directory)
{
  printf("\n\tLoading all Archetypes...\n\n");
  WIN32_FIND_DATA FindFileData;
  std::string dir = directory;
  std::string file = directory + "*.txt";

  std::wstring stemp = std::wstring(file.begin(), file.end());
  LPCWSTR sw = stemp.c_str();
  HANDLE hFind = FindFirstFile(sw, &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    return false;
  }
  else
  {

  }
  do
  {
    char ch[260];
    char DefChar = ' ';
    if (WideCharToMultiByte(CP_ACP, 0, FindFileData.cFileName, -1, ch, 260, &DefChar, NULL) == 0)
      return false;
    //A std:string  using the char* constructor.
    std::string str(ch);
    dir = directory;
    dir += str;
    if (LoadArchetypes(dir) == false)
      return false;
  } while (FindNextFile(hFind, &FindFileData));

  FindClose(hFind);

  return true;
}

bool Factory::ArchetypeExists(const std::string& name)
{
  return Archetypemap.count(name) ? true : false;
}

bool Factory::loadArchtypeComponents(std::istream& ser, GameObject* gameobj, const std::string& cmpname)
{
  /*while (!ser.eof())
  {
    std::map<std::string, ComponentRegistryBase*>::iterator it = Componentregistry.find(cmpname);
    if (it != Componentregistry.end())
    {
      Component* newcomp = (*it).second->Create();
      newcomp->SetParent(gameobj);
      refvar cmp = newcomp;
      cmp.DeSerialize(ser);
      gameobj->AddComponent(newcomp, true);
    }
  }

  while (ser.InHierachy())
  {
    std::map<std::string, ComponentRegistryBase*>::iterator it = Componentregistry.find(ser.GetCurElementName());

    if (it != Componentregistry.end())
    {

#ifdef FACTORYDEBUG
      std::cout << "cmp name: " << ser.GetCurElementName() << std::endl;
#endif

      Component* newcomp = (*it).second->Create();
      newcomp->SetParent(gameobj);
      
      //consider deserialize 
      //newcomp->Deserialize(ser);

      gameobj->AddComponent(newcomp, true);
    }
    else
    {
      std::cout << "Component: " + ser.GetCurElementName() + " not registered!" << std::endl;
      //ASSERTMSG(1, "Component: " + ser.GetCurElementName() + " not registered!");
    }
    if (!ser.IsDone())
      ser.GoToNextElement();
  }
  return true;*/
  return true;
}

void Factory::InitializeObjs(unsigned int start, unsigned int end)
{
  for (unsigned int i = start; i < end; ++i)
  {
    //GameObjectsMap[i]->InitComponents();
  }
}

bool Factory::LoadSceneFromFile(const std::string& fname, bool init)
{
  int count = 0;
  std::ifstream infile(fname);
  if (!infile.is_open())
    return false;

  TextSerializer::m_readlvl = 0;

  while (!infile.eof())
  {
    std::string name;

    std::getline(infile, name);
    Trim(name);

    if (TextSerializer::m_readlvl == 0)
    {
      GameObject* newobj = CloneFromArchType(name);

      while (!infile.eof())
      {
        std::string line = "";

        std::getline(infile, line);

        if (line == "")
          continue;

        if (line == "{" || line.find("{") != std::string::npos)
        {
          TextSerializer::m_readlvl++;
          continue;
        }

        if (line == "}" || line.find("}") != std::string::npos)
        {
          TextSerializer::m_readlvl--;
          if (TextSerializer::m_readlvl == 0)
            break;
          continue;
        }
         
        std::string copy = line;
        int pos = line.find_first_not_of(' ');
        std::string altvalue;
        if (pos != std::string::npos)
        {
          line = line.substr(pos, line.length());
          line = line.substr(0, line.find_first_of(' '));
          for (int i = 0; i <= copy.length(); ++i)
            infile.unget();
          if (line == "GameObject")
          {
            refvar refobj = *newobj;
            refobj.DeSerialize(infile);
            continue;
          }

          for (auto elem : newobj->GetComponentsMap())
          {
            if (elem.second->GetName() == line)
            {
              refvar cmp = *elem.second;
              cmp.SetMeta(META_STR(elem.second->GetName()));
              cmp.DeSerialize(infile);
              elem.second->Init();
              break;
            }
          }


        }
      }
     // newobj->SerializeObject(std::cout);
    }
   
  }
 
  return true;
}

bool Factory::LoadArchetypes(const std::string& fname)
{
  std::ifstream infile(fname);
  if (!infile.is_open())
    return false;

  //refvar newobj;
  //newobj.DeSerialize(infile);
  std::string name;

  std::getline(infile, name);
  Trim(name);
  
  if (Archetypemap.count(name))
  {
    std::cout << "ARCHTYPE: " << name << "EXISTS!" << std::endl;
  }

  GameObject* newobj = new GameObject(false, true);
  newobj->SetTypeName(name);
  while (!infile.eof())
  {
    std::string line = "";

    std::getline(infile, line);

    if (line == "" || line == "{")
      continue;

    if (line == "}")
      continue;
    std::string copy = line;
    int pos = line.find_first_not_of(' ');
    std::string altvalue;
    if (pos != std::string::npos)
    {
      line = line.substr(pos, line.length());
      line = line.substr(0, line.find_first_of(' '));

      std::map<std::string, ComponentRegistryBase*>::iterator it = Componentregistry.find(line);
      if (it != Componentregistry.end())
      {
        for (int i = 0; i <= copy.length(); ++i)
          infile.unget();

        Component* newcomp = (*it).second->Create();
        newcomp->SetParent(newobj);
        refvar cmp = *newcomp;
        cmp.SetMeta(META_STR(line));
        cmp.DeSerialize(infile);
       // cmp.Serialize(std::cout);
        newobj->AddComponent(newcomp, true);
      }
      
    }
  }
  Archetypemap[newobj->GetTypeName()] = newobj;
  return true;

/*  BBE::XMLSerializer ser;
  if (ser.LoadFile(fname) == false)
    return false;
  ser.SetToFirstChildElement();
  while (!ser.IsDone())
  {
    GameObject *newobj = New(GameObject, false, true);

    std::string objname(ser.GetCurElementName());
    newobj->SetTypeName(objname);
    if (Archetypemap.count(objname))
    {
      ASSERTMSG(1, "Archetype[" + objname + "] exists!");
    }

#ifdef FACTORYDEBUG
    std::cout << "Archetype name: " << objname << std::endl;
#endif

    Archetypemap[objname] = newobj;
    ser.SetToFirstChildElement();

    if (!loadArchtypeComponents(ser, newobj))
    {
      ASSERTMSG(1, "Loading Archetype[" + objname + "] failed!");
      return false;
    }
  }
  return true;*/
}

std::string Factory::GetLevelName(const std::string& s)
{
  return s.substr(s.find_last_of("/") + 1, s.length());
}

void Factory::RestoreFromBackup()
{
  GameObjectsMap = std::move(BackupGameObjectsMap);
  sameTypeVec = std::move(backupSameTypeVec);
}


void Factory::ClearObjectPool()
{
  for (auto& elem : GameObjectsMap)
  {
    delete elem.second;
  }
  GameObjectsMap.clear();
  /*
  for (auto& elem : sameTypeVec)
  {
  for (auto& elem2 : elem.second)
  {
  SAFE_DELETE(elem2);
  }
  }

  sameTypeVec.clear();
  }*/
}

/*
bool Factory::LoadSceneFromFile(const std::string& fname, bool init, unsigned int* startID, unsigned int* endID)
{
  BBE::XMLSerializer ser;
  if (ser.LoadFile(fname) == false)
    return false;


  if (g_collision->octree.hasChildren)
  {
    g_collision->octree.DestroyChildren();
    g_collision->octree.ClearObjs();
  }
  else
    g_collision->octree.ClearObjs();

  g_collision->colvec.clear();
  g_collision->m_updatedonce = false;
  ser.SetToFirstChildElement();
  g_graphics->DeserializeLevelGraphicsSettings(ser);
  ser.GoToNextElement();
  g_audioSystem->DeserializeSoundSettings(ser);
  ser.GoToNextElement();

  LoadContentsFromFile(ser, init, startID, endID);

  auto colmgr = CollisionManager::SingletonPtr;
  colmgr->Init();
  return true;
}

bool Factory::LoadAddonFromFile(const std::string & fname, bool init, unsigned int* startID, unsigned int* endID)
{
  BBE::XMLSerializer ser;
  if (ser.LoadFile(fname) == false)
    return false;

  ser.SetToFirstChildElement();
  ser.GoToNextElement();
  ser.GoToNextElement();

  LoadContentsFromFile(ser, init, startID, endID);

  return true;
}

bool Factory::LoadContentsFromFile(BBE::XMLSerializer & ser, bool init, unsigned int* startID, unsigned int* endID)
{
  unsigned int startId = GameObjId;
  std::vector<std::string> parentList;

  while (!ser.IsDone())
  {
    const std::string ObjType(ser.GetCurElementName());
    auto groupName = ser.GetAttributeFromCurrentS("group", std::string("Non-Group"));
    auto objName = ser.GetAttributeFromCurrentS("objName", ObjType);
    parentList.push_back(ser.GetAttributeFromCurrentS("parentName", ""));

    auto it = Archetypemap.find(ObjType);
    if (it != Archetypemap.end())
    {
      GameObject* newobj = it->second->Clone();

      newobj->SetId(GameObjId);
      //set unique name of the game obj
      newobj->m_uniqueName = objName;
      newobj->SetGroup(groupName);
      newobj->SetActive(true);
      GameObjectsMap[GameObjId++] = newobj;

      newobj->SetTypeName(ObjType);
      newobj->DeserializeObject(ser);

      newobj->SetUpBS(false);
      newobj->SetUpAABB(false);
      g_collision->AddToOctree(newobj);
      if (sameTypeVec.find(newobj->GetTypeName()) == sameTypeVec.end())
        sameTypeVec.insert({ newobj->GetTypeName(), BBE_vector(GameObject*)() });

      sameTypeVec[newobj->GetTypeName()].push_back(newobj);
      ser.GoToNextElement();
    }
    else
    {
      ERROR_MSG("Archetype[" + ObjType + "] does not exits! Cannot clone from archetype!");
      STOP_ENGINE;
      return false;
    }
  }
  for (unsigned int i = startId; i < GameObjId; ++i)
  {
    auto& parent = parentList[i - startId];
    if (parent != "")
    {
      GameObject* obj = g_factory->GetObjectByName(parent);
      if (obj)
        GameObjectsMap[i]->SetParent(*obj);
    }
  }
  if (init)
    InitializeObjs(startId, GameObjId);
  if (startID) *startID = startId;
  if (endID) *endID = GameObjId;

  return true;
}

bool Factory::SaveSceneToFile(const std::string& fname)
{
  BBE::XMLSerializer ser;
  ser.AddDeclaration();
  ser.SetToFirstChildElement();
  g_graphics->SerializeLevelGraphicsSettings(ser);
  g_audioSystem->SerializeSoundSettings(ser);

  for (auto& iter : sameTypeVec)
  {
    for (unsigned i = 0; i < iter.second.size(); ++i)
    {
      if (iter.first == "bullet" || iter.first == "firebullet" || iter.first == "HP_Bar" || iter.first == "HP_BG" || iter.first == "rebullet" || iter.first == "Sphere") continue;
      if (iter.second[i]->m_uniqueName.find("Dynamic_Emitter") != std::string::npos) continue;
      if (iter.second[i]->isEditorActive && !iter.second[i]->m_isDeleted)
        ser.AddElement(iter.first);
    }
  }

  ser.SetToFirstChildElement();
  ser.GoToNextElement();
  ser.GoToNextElement();
  for (auto& iter : sameTypeVec)
  {
    if (iter.first == "bullet" || iter.first == "firebullet" || iter.first == "rebullet" || iter.first == "HP_Bar" || iter.first == "HP_BG") continue;
    for (auto& objIter : iter.second)
    {
      if (objIter->m_uniqueName.find("Dynamic_Emitter") != std::string::npos) continue;
      if (!objIter->isEditorActive || objIter->m_isDeleted)
        continue;
      ser.AttachAttribute("group", objIter->GetGroupName().c_str());
      ser.AttachAttribute("objName", objIter->GetUniqueName().c_str());
      if (objIter->GetParent())
        ser.AttachAttribute("parentName", objIter->GetParent()->GetUniqueName().c_str());
      for (auto& compIter : objIter->components_)
      {
        if (compIter->showInEditor)
        {
          ser.AddElement(compIter->m_uniquename);
        }
      }
      ser.SetToFirstChildElement();
      for (auto& compIter : objIter->components_)
      {
        if (compIter->showInEditor)
        {
          compIter->Serialize(ser);
          ser.GoToNextElement();
        }
      }
      ser.ReturnToParent();
      ser.GoToNextElement();
    }
  }
  ser.SaveToFile(fname);
  return true;
}

bool Factory::SaveArchetypes(const std::string& fname, GameObject* obj)
{
  BBE::XMLSerializer ser;
  ser.AddDeclaration();
  ser.SetToFirstChildElement();
  obj->SerializeArchetype(ser);
  ser.SaveToFile(fname);
  return false;
}

bool Factory::SaveArchetypes(const std::string& fname)
{
  BBE::XMLSerializer ser;
  ser.AddDeclaration();

  return false;
}

BBE_vector(GameObject*)& Factory::GetObjectListByName(const std::string& type_name)
{
  // put a fake return to ignore compiler warning
  BBE_vector(GameObject*)* ret = nullptr;
  try
  {
    return sameTypeVec.at(type_name);
  }
  catch (...)
  {
    std::string msg = "Object of name: " + type_name + " not found!";
    MessageBoxA(0, msg.c_str(), "", MB_OK);
    std::exit(EXIT_FAILURE);
  }
  return *ret;
}

void Factory::SendMsgToObjects(const std::string& str, MessageObject& msg)
{
  auto it = sameTypeVec.find(str);

  if (it != sameTypeVec.end())
  {

    for (auto& elem : it->second)
    {
      if (!elem->IsActive()) continue;

      elem->Send(msg);
    }
  }
}

void Factory::MakeBackup()
{
  BackupGameObjectsMap = std::move(GameObjectsMap);
  backupSameTypeVec = std::move(sameTypeVec);
}


void Factory::DeleteMarkedObj()
{
  g_collision->RemoveDeletedCmp();
  g_componentGM->RemoveDeletedCmp();
  CMBase::cmpDeleted = false;

  std::vector<unsigned> idvec;
  for (auto & elem : GameObjectsMap)
  {
    if (elem.second->IsDeleted())
    {
      idvec.push_back(elem.first);

      //  sameTypeVec.erase(obj->GetTypeName());

    }
  }
  if (idvec.size() > 0)
  {
    for (unsigned int i = 0; i < idvec.size(); ++i)
    {
      auto obj = GameObjectsMap[idvec[i]];
      if (obj)
      {
        GameObjectsMap.erase(idvec[i]);
        auto it = std::find_if(sameTypeVec[obj->GetTypeName()].begin(), sameTypeVec[obj->GetTypeName()].end(), [&obj](GameObject* elem2)
        {
          if (elem2 == obj)
            return true;
          return false;
        });
        sameTypeVec[obj->GetTypeName()].erase(it);

        SAFE_DELETE(obj);
        obj = nullptr;
      }

    }
  }

}

BBE_map(std::string, BBE_vector(GameObject*))& Factory::GetObjectListMap()
{
  return sameTypeVec;
}


void Factory::DeleteGameObject(unsigned id)
{
  auto obj = GameObjectsMap.at(id);
  obj->RemoveAllComponents();
  auto it = std::find_if(sameTypeVec[obj->GetTypeName()].begin(), sameTypeVec[obj->GetTypeName()].end(), [&id](GameObject* elem)
  {
    if (elem->GetID() == id)
      return true;
    return false;
  });
  sameTypeVec[obj->GetTypeName()].erase(it);
  //  sameTypeVec.erase(obj->GetTypeName());
  GameObjectsMap.erase(id);
  g_componentGM->RemoveDeletedCmp();
  SAFE_DELETE(obj);
}
*/
///////////virutals//////////////////
void Factory::Load()
{
  printf("Factory Loading...\n");


  #define REGISTER_COMPONENT_AND_MGR(name, flag, duplicate, editorUpdate) \
if(flag){Factory::GetInstance()->RegisterComponent(#name, AllocateForComponentRegistry<name##>(ComponentID::name,editorUpdate));\
 ComponentGM::SingletonPtr->RegisterManager(ComponentID::name, AllocateManager<name>());}
 
  /*
#define REGISTER_COMPONENT_AND_MGR(name, flag, duplicate, editorUpdate) \
if(flag){Factory::GetInstance()->RegisterComponent(#name, AllocateForComponentRegistry<name##>(ComponentID::name,editorUpdate));\
}*/

#include"ComponentNames.h"
#undef  REGISTER_COMPONENT_AND_MGR

  //auto colmgr = New(CollisionManager);
  //REGISTER_MGR(ComponentID::CollisionComponent, colmgr);
  //LOG_MSG("Loading Archetypes...");
  LoadAllArchetypes("./Resource/Archetypes/");

 // LOG_MSG("Factory Loaded");
}
void Factory::Init()
{
  LoadSceneFromFile("./Resource/Levels/samplelvl.txt");
}