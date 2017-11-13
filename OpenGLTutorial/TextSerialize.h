#pragma once
#include <iostream>
#include <cassert>
#include <istream>
#include <string>
#include "RefelectionUtils.h"

void TextSerialize(std::ostream& os, refvar var);
void TextDeSerialize(std::istream& is, refvar& out);

void Padding(std::ostream& os);
void SkipLine(std::istream& is);
void Trim(std::string& str);

template <typename T>
void TextSerializePrim(std::ostream& os, refvar prim)
{
  Padding(os);
  prim.GetValue<RemQual<T>::type>();
  os << prim.GetValue<RemQual<T>::type>()  << std::endl;
}

template <typename T>
void TextDeSerializePrim(std::istream& is, refvar& out)
{
 // SkipLine(os);
  if (MetaCreator<RemQual<T>::type>::Get()->Name() == "std::string")
  {
    std::string& temp = out.GetValue<RemQual<std::string>::type>();
    std::getline(is, temp,'\n');
    if (temp.length() > 2)
    {
      int pos = temp.find_first_not_of(' ');
      if (pos != std::string::npos)
        temp = temp.substr(pos, temp.length());
    }
 
    std::string test = out.GetValue<RemQual<std::string>::type>();
  }
  else
  {
    T& temp = out.GetValue<RemQual<T>::type>();
    is >> temp;

  }



  //prim.SetValue(&line, 0, MetaCreator<RemQual<T>::type>::Get()->Size());
}


// Takes a pointer and returns a pointer offset in bytes
#define PTR_ADD( PTR, OFFSET ) \
  ((void *)(((char *)(PTR)) + (OFFSET)))

class TextSerializer
{
  
  static int m_padlvl;

public:
  static int m_readlvl;
  TextSerializer();
  static void AddPad()
  {
    m_padlvl++;
  }
  static void DecrementPad()
  {
    m_padlvl--;
  }
  static int PadLvl()
  {
    return m_padlvl;
  }

};
