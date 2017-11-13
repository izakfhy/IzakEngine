#include "MetaData.h"
#include <string>
#include <stdlib.h>
#include "refvar.h"
#include "TextSerialize.h"

Member::Member(std::string string, unsigned val, MetaData *meta,bool s) : name(string), offset(val), data(meta),
serializable(s), next(NULL)
{
}

Member::~Member()
{
}

const std::string& Member::Name(void) const
{
  return name;
}

unsigned Member::Offset(void) const
{
  return offset;
}

const MetaData *Member::Meta(void) const
{
  return data;
}

Member *& Member::Next(void)
{
  return next;
}

Member *const& Member::Next(void) const
{
  return next;
}

bool Member::IsSerializable() const
{
  return serializable;
}

void Member::SetIsSerializable(bool b)
{
  serializable = b;
}

MetaData::MetaData(std::string string, unsigned val) : name(string), size(val), members(NULL), lastMember(NULL)
{
}

MetaData::~MetaData()
{
  Member *temp = members;
  while (temp != nullptr)
  {
    Member* temp2 = temp;
    temp = temp->Next();
    delete temp2;
  }
}

void MetaData::Init(std::string string, unsigned val)
{
  name = string;
  size = val;
}

const std::string& MetaData::Name(void) const
{
  return name;
}

unsigned MetaData::Size(void) const
{
  return size;
}

void MetaData::AddMember(const Member *member)
{
  if (!members)
    members = const_cast<Member *>(member);
  else
    lastMember->Next() = const_cast<Member *>(member);

  lastMember = const_cast<Member *>(member);
}

bool MetaData::HasMembers(void) const
{
  return (members) ? true : false;
}


bool MetaData::HasMember(const std::string& memName) const
{
  if (!HasMembers())
    return false;

  Member* start = members;
  while (start)
  {
    if (start->Name() == memName)
      return true;

    start = start->Next();
  }
  return false;
}

void MetaData::Copy(void *dest, const void *src) const
{
  memcpy(dest, src, size);
}

void MetaData::Delete(void *data) const
{
  delete[] reinterpret_cast<char *>(data);
  data = NULL;
}

void *MetaData::NewCopy(const void *src) const
{
  void *data = new char[size];
  memcpy(data, src, size);
  return data;
}

void *MetaData::New(void) const
{
  return new char[size];
}

const Member *MetaData::Members(void) const
{
  return members;
}

void MetaData::PrintMembers(std::ostream& os) const
{
  const Member *mem = members;

  os << "Members for Meta: " << name << std::endl;

  while (mem)
  {
    os << "  " << mem->Meta()->Name() << " " << mem->Name() << std::endl;
    mem = mem->Next();
  }
}

void MetaData::SetSerialize(SerializeFn fn)
{
  serialize = fn;
}
void MetaData::SetDeSerialize(DeSerializeFn fn)
{
  deserialize = fn;
}

void MetaData::Serialize(std::ostream& os, refvar var) const
{
  if (serialize)
    serialize(os, var);
  else
    TextSerialize(os, var);
}

void MetaData::DeSerialize(std::istream& is, refvar out) const
{
  if (deserialize)
    deserialize(is, out);
  else
    TextDeSerialize(is, out);
}


const Member* MetaData::GetMember(const std::string& name) const
{
  Member *mem = members;

  while (mem)
  {
    if (mem->Name() == name)
      return mem;
    mem = mem->Next();
  }
  return nullptr;
}
Member* MetaData::GetMember(const std::string& name)
{
  Member *mem = members;

  while (mem)
  {
    if (mem->Name() == name)
      return mem;
    mem = mem->Next();
  }
  return nullptr;
}
