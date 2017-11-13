#include "refvar.h"
#include <cassert>
#include "var.h"

refvar::refvar() : varbase(NULL, NULL)
{

}

refvar::refvar(const MetaData *m, void *d) : varbase(m, d)
{
}

refvar::refvar(const refvar& rhs) : varbase(rhs.GetMeta(), rhs.Data())
{
  data = rhs.Data();
  meta = rhs.meta;
}

refvar::refvar(const var& rhs) : varbase(rhs.GetMeta(), rhs.Data())
{
}

refvar::refvar(const varbase& rhs) : varbase(rhs.GetMeta(), rhs.Data())
{

}


refvar& refvar::operator=(refvar rhs)
{
  meta = rhs.meta;
  data = rhs.data;
  return *this;
}

refvar& refvar::operator=(const var& rhs)
{
  meta = rhs.GetMeta();
  data = rhs.Data();
  return *this;
}
