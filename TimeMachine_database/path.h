#ifndef PATH_HXX
#define PATH_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#pragma db object
class path
{
public:
  path (const std::string& filePath)
      : filePath_ (filePath)
  {

  }

  const std::string&
  filePath () const
  {
    return filePath_;
  }

private:
  friend class odb::access;

  path () {}

  #pragma db id auto
  unsigned long id_;

  std::string filePath_;
};

#pragma db view object(path)
struct path_stat
{
  #pragma db column("count(" + path::id_ + ")")
  std::size_t count;

  #pragma db column("max(" + path::filePath_ + ")")
  std::string filePath;
};

#endif // PATH_HXX
