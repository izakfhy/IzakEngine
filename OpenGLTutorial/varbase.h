#pragma once
#include <iostream>
class MetaData;

class varbase
{
  protected:

    const MetaData *meta = NULL;
    void *data = NULL;
    varbase();
    varbase(const MetaData* meta_, void* data_);

  public:
    void SetMeta(const MetaData* d);
    const MetaData* GetMeta() const;

    template <typename T>
    T& GetValue(void);

    template <typename T>
    const T& GetValue(void) const;

    void *Data(void) const;


    void SetValue(void * val, unsigned offset, unsigned size);

    void Serialize(std::ostream& os) const;
    void DeSerialize(std::istream& is);
    varbase(const varbase& rhs);

};

template <typename T>
T& varbase::GetValue(void)
{
  return *reinterpret_cast<T *>(data);
}

template <typename T>
const T& varbase::GetValue(void) const
{
  return *reinterpret_cast<T *>(data);
}
