#ifndef SOURCE_HXX
#define SOURCE_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#pragma db object
class source
{
public:
  source (const std::string& name,
          const std::string& adress)
      : name_ (name), adress_ (adress)
  {
  }

  const std::string&
  name () const
  {
    return name_;
  }

  const std::string&
  adress () const
  {
    return adress_;
  }

private:
  friend class odb::access;

  source () {}

  #pragma db id auto
  unsigned long id_;

  std::string name_;
  std::string adress_;
};

#pragma db view object(source)
struct source_stat
{
  #pragma db column("count(" + source::id_ + ")")
  std::size_t count;

  #pragma db column("min(" + source::name_ + ")")
  std::string name;

  #pragma db column("max(" + source::adress_ + ")")
  std::string adress;
};

#endif // SOURCE_HXX
