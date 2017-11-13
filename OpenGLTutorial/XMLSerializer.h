#pragma once
#include <string>
#include "tinyxml2.h"

enum class BBEXML
{
  SUCCESS = 0,
  END
};

class XMLSerializer
{
  tinyxml2::XMLElement * curElement, *rootElement, *prevElement;

public:
  mutable const tinyxml2::XMLAttribute* curAttr, *prevAttr;
  tinyxml2::XMLDocument xmlDoc;


  ~XMLSerializer();
  XMLSerializer() :curElement(nullptr), rootElement(nullptr), prevElement(nullptr)
    , curAttr(nullptr), prevAttr(nullptr)
  {}

  ///////////////READ FUNCTIONS/////////////////////
  //read the current element and jump to next element automatically, put false to stop
  void ReadElement(int& val, bool next = true);
  float ReadElement(float& val, bool next = true);
  bool ReadElement(bool& val, bool next = true);
  std::string& ReadElement(std::string& val, bool next = true);
  void Serialize(const std::string& name, const std::string& attrb, int& val){}

  /////////SETTORS//////////
  //set to child lvl node - 1 level down
  void SetToChildNode(const std::string& parentn, const std::string& node);
  //goes straight to first child
  void SetToFirstChildElement();
  void SetToFirstChildElement(const std::string& elename);
  void SetToLastChildElement();
  void SetToFirstAttribute();
  //set to current hierachy lvl node
  bool SetCurNode(const std::string& node);

  /////////GETTORS///////////
  int GetAttributeFromCurrent(const std::string& attrb, int defaultval) const;
  bool GetAttributeFromCurrent(const std::string& attrb, bool defaultval) const;
  std::string GetAttributeFromCurrentS(const std::string& attrb, const std::string& defaultval) const;
  BBEXML GetAttributeFromCurrent(std::string& val) const;
  float GetAttributeFromCurrent(const std::string& attrb, float defaultval) const;

  bool IsDone() const;
  bool HasChildElement() const;
  bool IsLastElement() const;
  bool InHierachy();
  bool IsLastAttrOfCurElement();
  tinyxml2::XMLElement * GetRootElement();
  tinyxml2::XMLElement * GetCurElement();
  std::string GetCurElementName();

  void GoToNextElement();
  void ReturnToParent();

  //////////////WRITE FUNCTIONS/////////////////////
  void AddDeclaration();
  bool SaveToFile(const std::string& fname);
  void AddElement(const std::string& elename);

  template <typename T>
  void AttachAttribute(const std::string& atrbname, const T& val)
  {
    curElement->SetAttribute(atrbname.c_str(), val);
  }

  void AddElement(const std::string& elename, const int val);
  void AddElement(const std::string& elename, const float val);
  void AddElement(const std::string& elename, const std::string& val);

  bool LoadFile(const std::string& fname);
};