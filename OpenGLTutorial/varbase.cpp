#include "varbase.h"
#include "MetaData.h"
#include "refvar.h"
#include "TextSerialize.h"


varbase::varbase() : meta(NULL), data(NULL)
{

}

varbase::varbase(const MetaData* meta_, void* data_)
  : meta(meta_), data(data_)
{

}

void varbase::SetMeta(const MetaData* d)
{
  meta = d;
}
const MetaData* varbase::GetMeta() const
{
  return meta;
}

void *varbase::Data(void) const
{
  return data;
}


void varbase::Serialize(std::ostream& os) const
{
  meta->Serialize(os, refvar(meta, data));
}

varbase::varbase(const varbase& rhs) :data(rhs.data), meta(rhs.meta)
{

}

void varbase::DeSerialize(std::istream& is)
{
  if (meta)
  {
    meta->DeSerialize(is,*this);
  }


  //SetValue(v.Data(), v.GetMeta()->Size());
}

void varbase::SetValue(void * val,unsigned offset, unsigned size)
{
  memcpy(PTR_ADD(data, offset), val, size);
}