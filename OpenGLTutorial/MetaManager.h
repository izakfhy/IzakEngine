#pragma once
#include <map>
#include <string>
#include "MetaData.h"

//
// MetaManager
// Purpose: Just a collection of some functions for management of all the
//          various MetaData objects.
//
class MetaManager
{
public:
  typedef std::map<std::string, const MetaData *> MetaMap;

  // Insert a MetaData into the map of objects
  static void RegisterMeta(const MetaData *instance)
  {
    GetMap().insert({ instance->Name(), instance });
  }

  // Retrieve a MetaData instance by string name from the map of MetaData objects
  static const MetaData *Get(std::string name)
  {
    // NULL if not found
    return GetMap().at(name);
  }
  // Safe and easy singleton for map of MetaData objects
  static MetaMap& GetMap(void)
  {
    // Define static map here, so no need for explicit definition
    static MetaMap map;
    return map;
  }
};
