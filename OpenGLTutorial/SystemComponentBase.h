/******************************************************************************/
/*!
\file	  SystemComponent.h
\author Izak Foong, foong.h, 440002413
\par    foong.h\@digipen.edu
\date   12/05/2014
\brief
The messanging system that stores function within a given event and calls this
function with the message send to the event

All content © 2015 DigiPen Institute of Technology Singapore. All Rights Reserved
*/
/******************************************************************************/
#pragma once

#include <string>

//Macro trick to make component names enums from the file SystemComponentNames.h
#define REGISTER_SYSTEM_COMPONENT_NAME(x) x,
enum class SYS_CMP
{
#include "SystemComponentNames.h"
};
#undef REGISTER_SYSTEM_COMPONENT_NAME

//Macro trick to make component names strings from the file SystemComponentNames.h
#define REGISTER_SYSTEM_COMPONENT_NAME(x) #x,
const std::string SystemComponentNames[] =
{
#include "SystemComponentNames.h"W
};
#undef REGISTER_SYSTEM_COMPONENT_NAME


class SystemComponentBase
{
public:
  virtual ~SystemComponentBase()
  {};

  virtual void Init() {}
  virtual void Load() {}
  virtual void Update() {}
  virtual void Free() {}
  virtual void Unload() {}
};
