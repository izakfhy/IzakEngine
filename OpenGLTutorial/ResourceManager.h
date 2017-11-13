#pragma once
#include <map>
#include <memory>
#include "Resource.h"
#include "SystemComponentBase.h"

namespace IZE
{
  typedef std::map<std::string, std::shared_ptr<IZE::Resource>> ResourceMap;
  class ResourceManager : public SystemComponentBase
  {

    ResourceMap resourcemap;
  public:

    static ResourceManager* SingletonPtr;
    static ResourceManager* GetInstance();
    ResourceMap& GetResourceMapContainer();

    ResourceManager()
    {
      SingletonPtr = this;
      resourcemap.clear();
    }
    ResourceManager::~ResourceManager();

    template<typename T>
    std::shared_ptr<T> GetResource(const std::string& path)
    {
      auto it = resourcemap.find(path);
      if (it != resourcemap.end())
      {
        resourcemap[path]->SetName(path);
        return std::static_pointer_cast<T>((*it).second);
      }

      std::shared_ptr<T> ptr(std::make_shared<T>());
      if (ptr->Initialize(path) == false)
      {
        return nullptr;
      }

      resourcemap[path] = ptr;
      resourcemap[path]->SetName(path);
      return ptr;
    }

  };
}
