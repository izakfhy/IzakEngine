#pragma once

#include <vector>
#include <string>
#include "RemQual.h"


class refvar;
typedef void(*SerializeFn)(std::ostream&, refvar);
typedef void(*DeSerializeFn)(std::istream&, refvar&);

#define DEFINE_META( T ) \
  MetaCreator<RemQual<T>::type> NAME_GENERATOR( )( #T, sizeof( T ) ); \
  RemQual<T>::type *T::NullCast( void ) { return reinterpret_cast<RemQual<T>::type *>(NULL); } \
  void T::AddMember( std::string name, unsigned offset, MetaData *data, bool s ) { return MetaCreator<RemQual<T>::type>::AddMember( name, offset, data,s ); } \
  void MetaCreator<RemQual<T>::type>::RegisterMetaData( void ) { MetaManager::RegisterMeta(MetaCreator<RemQual<T>::type>::Get()); T::RegisterMetaData( ); } \
  void T::RegisterMetaData( void )

// META_DATA
// Purpose : This macro goes on the inside of a class within the public section. It declares
//           a few member functions for use by the MetaData system to retrieve information about
//           the class.
#define META_DATA( T ) \
  static void AddMember( std::string name, unsigned offset, MetaData *data, bool s ); \
  static RemQual<T>::type *NullCast( void ); \
  static void RegisterMetaData( void )

// Defines the RegisterMetaData for you
#define DEFINE_META_POD( T ) \
  MetaCreator<RemQual<T>::type> NAME_GENERATOR( )( #T, sizeof( T ) ); \
  void MetaCreator<RemQual<T>::type>::RegisterMetaData( void ) \
  { \
    MetaCreator<RemQual<T>::type>::SetSerializeFn( TextSerializePrim<RemQual<T>::type> ); \
    MetaCreator<RemQual<T>::type>::SetDeSerializeFn( TextDeSerializePrim<RemQual<T>::type> ); \
  }

#define ADD_MEMBER( MEMBER, SERIAL ) \
  AddMember( #MEMBER, (unsigned)(&(NullCast( )->MEMBER)), META( NullCast( )->MEMBER), SERIAL)

#define PARENT_META(T) \
  auto temp = [](){ \
  auto meta = META_TYPE(T);\
  auto start = meta->Members(); \
    while(start!= nullptr){ AddMember(start->Name(), start->Offset(), const_cast<MetaData*>(start->Meta()), start->IsSerializable()); \
    start = start->Next();} \
}; \
temp()

#define SET_SERIALIZE( FN ) \
  MetaCreator<RemQual<TYPE>::type>::SetSerializeFn( FN )

#define PASTE_TOKENS_2( _, __ ) _##__
#define PASTE_TOKENS( _, __ ) PASTE_TOKENS_2( _, __ )
#define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
#define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )

//
// META_TYPE
// Purpose: Retrieves the proper MetaData instance of an object by type.
//
#define META_TYPE( T ) (MetaCreator<RemQual<T>::type>::Get( ))

//
// META
// Purpose: Retrieves the proper MetaData instance of an object by an object's type.
//
#define META( OBJECT ) (MetaCreator<RemQual<decltype( OBJECT )>::type>::Get( ))

//
// META_STR
// Purpose : Finds a MetaData instance by string name
//
#define META_STR( STRING ) (MetaManager::Get( STRING ))

class MetaData;

//
// Member
// Purpose: Stores information (name and offset of member) about a data member of a specific class. Multiple
//          Member objects can be stored in MetaData objects within a std::vector.
//
class Member
{
public:
  Member(std::string string, unsigned val, MetaData *meta,bool s);
  ~Member();

  const std::string &Name(void) const; // Gettor for name
  unsigned Offset(void) const; // Gettor for offset
  const MetaData *Meta(void) const; // Gettor for data

  Member *& Next(void);
  Member *const& Next(void) const;

  bool Member::IsSerializable() const;
  void Member::SetIsSerializable(bool b);

private:
  std::string name;
  unsigned offset;
  const MetaData *data;
  Member *next;
  bool serializable;
};

//
// MetaData
// Purpose: Object for holding various info about any C++ type for the MetaData reflection system.
//
class MetaData
{
public:
  MetaData(std::string string = "", unsigned val = 0);
  ~MetaData();

  void Init(std::string string, unsigned val);

  const std::string& Name(void) const;
  unsigned Size(void) const;
  void AddMember(const Member *member);
  bool HasMembers(void) const;

  bool HasMember(const std::string& memName) const;

  void Copy(void *data, const void *src) const;
  void Delete(void *data) const;
  void *NewCopy(const void *src) const;
  void *New(void) const;

  const Member *Members(void) const;
  void PrintMembers(std::ostream& os) const;

  void SetSerialize(SerializeFn fn = NULL);

  void SetDeSerialize(DeSerializeFn fn = NULL);

  void Serialize(std::ostream& os, refvar) const;
  void DeSerialize(std::istream& is, refvar out) const;

  bool IsSerializable() const;
  void SetIsSerializable(bool b);

  const Member* GetMember(const std::string& name) const;
  Member* GetMember(const std::string& name);
private:
  SerializeFn serialize;
  DeSerializeFn deserialize;
  Member *members;
  Member *lastMember;
  std::string name;
  unsigned size;
};

template <typename Metatype>
class MetaCreator
{
public:
  MetaCreator(std::string name, unsigned size)
  {
    Init(name, size);
  }

  static void Init(std::string name, unsigned size)
  {
    Get()->Init(name, size);
    RegisterMetaData();
  }

  static void AddMember(std::string memberName, unsigned memberOffset, MetaData *meta, bool s)
  {
    Get()->AddMember(new Member(memberName, memberOffset, meta,s));
  }

  static void SetSerializeFn(SerializeFn fn)
  {
    Get()->SetSerialize(fn);
  }

  static void SetDeSerializeFn(DeSerializeFn fn)
  {
    Get()->SetDeSerialize(fn);
  }

  static Metatype *NullCast(void)
  {
    return reinterpret_cast<Metatype *>(NULL);
  }

  static void RegisterMetaData(void);

  // Ensure a single instance can exist for this class type
  static MetaData *Get(void)
  {
    static MetaData instance;
    return &instance;
  }
};
