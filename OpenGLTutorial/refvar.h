#pragma once
#include <algorithm>
#include <typeinfo>
#include "varbase.h"

class MetaData;
class var;

class refvar : public varbase
{
public:
  refvar();
  refvar(const MetaData *m, void *d);
  refvar(const var& rhs);

  template <typename T>
  refvar(const T& value);

  refvar(const refvar& rhs);
  refvar(const varbase& rhs);

  template <typename T>
  refvar& operator=(const T& rhs);

  refvar& operator=(refvar rhs);
  refvar& refvar::operator=(const var& rhs);

};

template <typename T>
refvar::refvar(const T& value)
  :varbase(META_TYPE(T), const_cast<T *>(&value))
{
 // std::cout << typeid(T).name() << std::endl;
}

template <typename T>
refvar& refvar::operator = (const T& rhs)
{
  meta = META_TYPE(T);
  data = const_cast<T *>(&rhs);
  return *this;
}