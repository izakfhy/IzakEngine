#include "ComponentManager.h"



bool CMBase::cmpDeleted = false;

CMBase::~CMBase()
{

}


void CMBase::SetManagerActive(const bool a)
{
  active = a;
}

bool CMBase::IsActive()const
{
  return active;
}
