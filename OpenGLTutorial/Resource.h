#pragma once

#include <string>


namespace IZE
{
  class Resource
  {
    std::string mname;
  public:
   
    void SetName(const std::string& path)
    {
      mname = path;
    }
    std::string GetName()const
    {
      return mname;
    }
    Resource()
    {
    }
    virtual bool Initialize(const std::string& path) = 0;
    virtual ~Resource(){};
  };


}
