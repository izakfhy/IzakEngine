/* Start Header ***************************************************************/
/*!
\file   XMLSerializer.cpp
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
#include <iostream>
#include "XMLSerializer.h"

  bool XMLSerializer::SaveToFile(const std::string& fname)
  {
    tinyxml2::XMLError eResult = xmlDoc.SaveFile(fname.c_str());
    if (eResult != tinyxml2::XML_NO_ERROR)
    {
      printf("Unable to save to file!\n");
      return false;
    }

    return true;
  }

  void XMLSerializer::GoToNextElement()
  {
    prevElement = curElement;
    curElement = curElement->NextSiblingElement();
    SetToFirstAttribute();
  }

  bool XMLSerializer::IsDone() const
  {
    return !curElement;
  }

  bool XMLSerializer::HasChildElement() const
  {
    return curElement->FirstChildElement() ? true : false;
  }

  bool XMLSerializer::IsLastElement() const
  {
    return !curElement->NextSiblingElement();
  }

  int XMLSerializer::GetAttributeFromCurrent(const std::string& attrb, int defaultval) const
  {
    int val = defaultval;
    if (curElement != nullptr)
    {
      auto temp = curElement->Attribute(attrb.c_str());
      if (temp)
        val = atoi(temp);
    }
    else if (prevElement)
    {
      auto temp = prevElement->Attribute(attrb.c_str());

      if (temp)
        val = atoi(temp);
    }

    return val;
  }

  BBEXML XMLSerializer::GetAttributeFromCurrent(std::string& val) const
  {

    if (!curElement && !prevElement)
    {
      std::cout << "End of Hierachy reached! There is nothing else to read!" << std::endl;
      return BBEXML::END;
    }

    if (curAttr != nullptr)
    {
      val = std::string(curAttr->Value());
      curAttr = curAttr->Next();
      return BBEXML::SUCCESS;
    }

    else
    {
      if (prevAttr != nullptr)
      {
        val = std::string(prevAttr->Value());
        prevAttr = prevAttr->Next();
        return BBEXML::SUCCESS;
      }

      else
      {
        val = "";
        std::cout << "End of Hierachy reached! There is nothing else to read!" << std::endl;
        return BBEXML::END;
      }
    }
    return BBEXML::SUCCESS;
  }

  float XMLSerializer::GetAttributeFromCurrent(const std::string& attrb, float defaultval) const
  {
    float val = defaultval;
    if (curElement != nullptr)
    {
      auto temp = curElement->Attribute(attrb.c_str());
      if (temp)
        val = std::stof(temp);
    }
    else if (prevElement)
    {
      auto temp = prevElement->Attribute(attrb.c_str());

      if (temp)
        val = std::stof(temp);
    }

    return val;
  }

  bool XMLSerializer::GetAttributeFromCurrent(const std::string& attrb, bool defaultval) const
  {
    bool val = defaultval;
    if (curElement != nullptr)
    {
      auto temp = curElement->Attribute(attrb.c_str());
      if (temp)
        val = !(strcmp(curElement->Attribute(attrb.c_str()), "0") == 0);
    }
    else if (prevElement)
    {
      auto temp = prevElement->Attribute(attrb.c_str());

      if (temp)
        val = !(strcmp(prevElement->Attribute(attrb.c_str()), "0") == 0);
    }

    return val;
  }

  std::string XMLSerializer::GetAttributeFromCurrentS(const std::string& attrb, const std::string& defaultval) const
  {

    std::string val = defaultval;
    if (curElement != nullptr)
    {
      auto temp = curElement->Attribute(attrb.c_str());
      if (temp)
        val = temp;
    }
    else if (prevElement)
    {
      auto temp = prevElement->Attribute(attrb.c_str());
      if (temp)
        val = temp;
    }

    return val;
  }

  float XMLSerializer::ReadElement(float& val, bool next)
  {
    if (curElement != nullptr)
    {
      if (curElement->QueryFloatText(&val) != tinyxml2::XML_SUCCESS)
        printf("read to float error!\n");

      if (next)
        curElement = curElement->NextSiblingElement();

      else
        ReturnToParent();
    }

    else
    {
      if (prevElement != nullptr)
      {
        if (prevElement->QueryFloatText(&val) != tinyxml2::XML_SUCCESS)
          printf("read to float error!\n");
      }

      else
      {
        std::cout << "End of Hierachy reached! There is nothing else to read!" << std::endl;
      }
    }
    return val;
  }

  std::string& XMLSerializer::ReadElement(std::string& val, bool next)
  {
    if (curElement != nullptr)
    {
      SetToFirstAttribute();
      val = curElement->GetText();

      if (next)
        curElement = curElement->NextSiblingElement();

      else
        ReturnToParent();
    }

    else
    {
      if (prevElement != nullptr)
      {
        val = prevElement->GetText();
      }

      else
      {
        std::cout << "End of Hierachy reached! There is nothing else to read!" << std::endl;
      }
    }
    return val;
  }

  bool XMLSerializer::ReadElement(bool& val, bool next)
  {
    if (curElement != nullptr)
    {
      if (curElement->QueryBoolText(&val) != tinyxml2::XML_SUCCESS)
        printf("read to bool error!\n");

      if (next)
        curElement = curElement->NextSiblingElement();

      else
        ReturnToParent();
    }
    else
    {
      if (prevElement != nullptr)
      {
        if (prevElement->QueryBoolText(&val) != tinyxml2::XML_SUCCESS)
          printf("read to float error!\n");
      }

      else
      {
        std::cout << "End of Hierachy reached! There is nothing else to read!" << std::endl;
      }
    }
    return val;
  }

  void XMLSerializer::ReadElement(int& val, bool next)
  {
    if (curElement != nullptr)
    {
      if (curElement->QueryIntText(&val) != tinyxml2::XML_SUCCESS)
        printf("read to bool error!\n");

      if (next)
        curElement = curElement->NextSiblingElement();

      else
        ReturnToParent();
    }
    else
    {
      if (prevElement != nullptr)
      {
        if (prevElement->QueryIntText(&val) != tinyxml2::XML_SUCCESS)
          printf("read to float error!\n");

      }

      else
      {
        std::cout << "End of Hierachy reached! There is nothing else to read!" << std::endl;
      }
    }
  }

  void XMLSerializer::SetToChildNode(const std::string& parentn, const std::string& node)
  {

  }

  bool XMLSerializer::SetCurNode(const std::string& node)
  {
    auto tempcur = curElement->Parent()->FirstChildElement(node.c_str());
    if (tempcur)
    {
      curElement = tempcur;
      return true;
    }

    return false;

  }

  void XMLSerializer::SetToFirstChildElement()
  {
    if (curElement)
    {
      curElement = curElement->FirstChildElement();
      curAttr = curElement->FirstAttribute();
    }
    else if (prevElement)
    {
      prevElement = prevElement->FirstChildElement();
      prevAttr = prevElement->FirstAttribute();
    }
  }

  void XMLSerializer::SetToFirstChildElement(const std::string& elename)
  {
    curElement = curElement->FirstChildElement(elename.c_str());
    SetToFirstAttribute();
  }

  void XMLSerializer::SetToLastChildElement()
  {
    curElement = curElement->LastChildElement();
    SetToFirstAttribute();
  }

  void XMLSerializer::ReturnToParent()
  {
    if (curElement)
      curElement = curElement->Parent()->ToElement();
    else
      curElement = prevElement->Parent()->ToElement();
    SetToFirstAttribute();
  }

  bool XMLSerializer::InHierachy()
  {
    if (curElement && curElement->NextSiblingElement() == NULL)
    {
      prevElement = curElement;
      curElement = curElement->NextSiblingElement();
      curAttr = prevElement->FirstAttribute();
      return true;
    }

    else if (curElement == NULL)
    {
      curElement = prevElement;
      ReturnToParent();
      curElement = curElement->NextSiblingElement();
      return false;
    }

    return true;
  }

  std::string XMLSerializer::GetCurElementName()
  {
    if (curElement)
      return curElement->Name();
    else if (prevElement)
      return prevElement->Name();

    return std::string(); //to disable warning
  }

  bool XMLSerializer::LoadFile(const std::string& fname)
  {
    if (xmlDoc.LoadFile(fname.c_str()) == tinyxml2::XML_SUCCESS)
    {
      rootElement = xmlDoc.FirstChildElement();
      if (!rootElement)
      {
        printf("ERROR reading in first element!\n");
        return false;
      }
      curElement = rootElement;

      return true;
    }
    return false;
  }

  tinyxml2::XMLElement *XMLSerializer::GetRootElement()
  {
    return rootElement;
  }

  tinyxml2::XMLElement * XMLSerializer::GetCurElement()
  {
    return curElement;
  }

  XMLSerializer::~XMLSerializer()
  {
  }

  void XMLSerializer::AddElement(const std::string& elename, const int val)
  {
    if (!rootElement)
    {
      rootElement = xmlDoc.NewElement("Root");
      xmlDoc.InsertEndChild(rootElement);
      curElement = rootElement;
    }

    tinyxml2::XMLElement * pElement = xmlDoc.NewElement(elename.c_str());
    pElement->SetText(val);
    curElement->InsertEndChild(pElement);
  }

  void XMLSerializer::AddElement(const std::string& elename, const float val)
  {
    if (!rootElement)
    {
      rootElement = xmlDoc.NewElement("Root");
      xmlDoc.InsertEndChild(rootElement);
      curElement = rootElement;
    }

    tinyxml2::XMLElement * pElement = xmlDoc.NewElement(elename.c_str());
    pElement->SetText(val);
    curElement->InsertEndChild(pElement);
  }

  void XMLSerializer::AddElement(const std::string& elename, const std::string& val)
  {
    if (!rootElement)
    {
      rootElement = xmlDoc.NewElement("Root");
      xmlDoc.InsertEndChild(rootElement);
      curElement = rootElement;
    }

    tinyxml2::XMLElement * pElement = xmlDoc.NewElement(elename.c_str());
    pElement->SetText(val.c_str());
    curElement->InsertEndChild(pElement);
  }

  //creates empty element
  void XMLSerializer::AddElement(const std::string& elename)
  {
    if (!rootElement)
    {
      rootElement = xmlDoc.NewElement("Root");
      xmlDoc.InsertEndChild(rootElement);
      curElement = rootElement;
    }

    tinyxml2::XMLElement * pElement = xmlDoc.NewElement(elename.c_str());
    curElement->InsertEndChild(pElement);
  }

  void XMLSerializer::AddDeclaration()
  {
    tinyxml2::XMLDeclaration* decl = xmlDoc.NewDeclaration("xml version=\"1.0\" ");
    xmlDoc.InsertFirstChild(decl);
  }

  bool XMLSerializer::IsLastAttrOfCurElement()
  {
    if (curAttr && curAttr->Next() == NULL)
    {
      prevAttr = curAttr;
      curAttr = curAttr->Next();
      return false;
    }

    else if (curAttr && curAttr->Next() != NULL)
    {
      return false;
    }

    else if (curAttr == NULL)
    {
      curAttr = prevAttr;
      return true;
    }
    else if (curAttr == NULL && curElement == nullptr)
    {
      curAttr = prevAttr;
      return true;
    }

    return true;
  }

  void XMLSerializer::SetToFirstAttribute()
  {
    if (curElement)
    {
      curAttr = curElement->FirstAttribute();
    }
  }

