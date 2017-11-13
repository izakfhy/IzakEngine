/* Start Header ***************************************************************/
/*!
\file   ResourceManager.cpp
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
#include "ResourceManager.h"

IZE::ResourceManager* IZE::ResourceManager::SingletonPtr = nullptr;
namespace IZE
{


  ResourceManager* ResourceManager::GetInstance()
  {
    if (SingletonPtr == nullptr)
      SingletonPtr = new ResourceManager();

    return SingletonPtr;
  }

  ResourceManager::~ResourceManager()
  {
    resourcemap.clear();
    //delete SingletonPtr;
  }

  ResourceMap& ResourceManager::GetResourceMapContainer()
  {
    return resourcemap;
  }

}