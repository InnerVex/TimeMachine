#ifndef PERSON_HXX
#define PERSON_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#pragma db object
class person
{
public:
  person (const std::string& index,
          const std::string& dateTime,
          unsigned short available)
      : index_ (index), dateTime_ (dateTime), available_ (available)
  {
  }

  const std::string&
  index () const
  {
    return index_;
  }

  const std::string&
  dateTime () const
  {
    return dateTime_;
  }

  unsigned short
  available () const
  {
    return available_;
  }

  void
  available (unsigned short available)
  {
    available_ = available;
  }

private:
  friend class odb::access;

  person () {}

  #pragma db id auto
  unsigned long id_;

  std::string index_;
  std::string dateTime_;
  unsigned short available_;
};

#pragma db view object(person)
struct person_stat
{
  #pragma db column("count(" + person::id_ + ")")
  std::size_t count;

  #pragma db column("min(" + person::available_ + ")")
  unsigned short min_age;

  #pragma db column("max(" + person::available_ + ")")
  unsigned short max_age;
};

#endif // PERSON_HXX
