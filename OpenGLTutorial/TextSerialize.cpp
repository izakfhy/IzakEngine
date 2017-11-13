
#include "TextSerialize.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include "MetaData.h"
#include "refvar.h"
#include <algorithm>
#include <locale>


static unsigned pad_level = 0;
int TextSerializer::m_padlvl = 0;
int TextSerializer::m_readlvl = 0;

void Padding(std::ostream& os)
{
  for (unsigned i = 0; i < TextSerializer::PadLvl(); ++i)
    os << "  ";
}

void SkipLine(std::istream& is)
{
  std::string line;
  std::getline(is, line);

  if (line == "{" || line.find("{") != std::string::npos)
  {
    TextSerializer::m_readlvl++;
  }

  if (line == "}" || line.find("}") != std::string::npos)
  {
    TextSerializer::m_readlvl--;
  }
}

void Trim(std::string& str)
{
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}

void TextDeSerialize(std::istream& is,refvar& out)
{
  const MetaData* meta = nullptr;
  std::string name;

  std::getline(is, name);
  Trim(name);

  SkipLine(is);
  if (name == "")
    return;

  meta = META_STR(name);
  if (!meta || meta->Name() != out.GetMeta()->Name())
    return;

  out.SetMeta(meta);
  while (true)
  {
    if (is.eof())
      break;

    std::string line = "";
    is >> line;


    if (line == "" || line == "{")
      continue;

    if (line == "{" || line.find("{") != std::string::npos)
    {
      TextSerializer::m_readlvl++;
      continue;
    }

    if (line == "}")
    {
      TextSerializer::m_readlvl--;
      return;
    }

    int pos = line.find_first_not_of(' ');
    std::string altvalue;
    if (pos != std::string::npos)
    {
      line = line.substr(pos, line.length());
      line = line.substr(0, line.find_first_of(' '));
      if (meta->HasMember(line))
      {
        Member* mem = const_cast<MetaData*>(meta)->GetMember(line);
        refvar memvar(mem->Meta(),PTR_ADD(out.Data(),mem->Offset()));
        memvar.DeSerialize(is);
 
      }
      else
        break;

    }

  }
}


void TextSerialize(std::ostream& os, refvar var)
{
  const MetaData *meta = var.GetMeta();
  void *data = var.Data();

  assert(meta->HasMembers()); // This type does not have a SerializePrim associated with it for some reason.

  const Member *mem = meta->Members();
  Padding(os);
  os << meta->Name()  << std::endl;
  Padding(os);
  os << "{" << std::endl;

  while (mem)
  {
    if (!mem->IsSerializable())
    {
      mem = mem->Next();
      continue; 
    }
    TextSerializer::AddPad();
   // ++pad_level;
    void *offsetData = PTR_ADD(var.Data(), mem->Offset());
    Padding(os);

    os << mem->Name() << " ";
    mem->Meta()->Serialize(os, refvar(mem->Meta(), offsetData));
    mem = mem->Next();

    //Padding(os);
    //os << std::endl;
    TextSerializer::DecrementPad();
    //--pad_level;
  }

  Padding(os);
  os << "}" << std::endl;
}
