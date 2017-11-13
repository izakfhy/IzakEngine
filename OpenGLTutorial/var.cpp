#include "MetaData.h"
#include "var.h"
#include <stdlib.h>
#include <cassert>

var::var() : varbase(NULL, NULL)
{
}

var::var(const MetaData *m, void *d) : varbase(m, d)
{

}


var& var::operator=(var rhs)
{
  if (this == &rhs)
    return *this;

  if (meta)
  {
    if (meta == rhs.meta)
      meta->Copy(data, rhs.data);
    else
    {
      assert(rhs.meta); // Cannot make an instance of NULL meta!

      meta->Delete(data);
      meta = rhs.GetMeta();

      // We require a new copy if meta does not match!
      if (meta)
        data = meta->NewCopy(&rhs.data);
    }
  }

  return *this;
}