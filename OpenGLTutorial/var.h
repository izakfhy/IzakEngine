#pragma once
#include "MetaData.h"
#include "varbase.h"

class var : public varbase
{
  public:
    template <typename T>
    var(const T& value);

    var();
    //var(const var& rhs);
    var(const MetaData *m, void *d);


    template <typename TYPE>
    var& operator=(const TYPE& rhs);

    var& operator=(var rhs);

};

template <typename T>
var::var(const T& value) : varbase(META_TYPE(T), NULL)
{
  data = meta->NewCopy(&value);
}

template <typename TYPE>
var& var::operator = (const TYPE& rhs)
{
  // We require a new copy if meta does not match!
  if (meta != META_TYPE(T))
  {
    assert(META_TYPE(T)); // Cannot create instance of NULL meta!

    meta->Delete(data);
    meta = META_TYPE(T);
    data = meta->NewCopy(&rhs);
  }
  else
  {
    meta->Copy(data, &rhs);
  }
  return *this;
}