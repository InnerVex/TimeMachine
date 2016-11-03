#ifndef FILE_HXX
#define FILE_HXX

#include <string>
#include <cstddef> // std::size_t

#include <vector>

#include <odb/core.hxx>
#include <odb/tr1/memory.hxx>
#include "path.h"
#include "source.h"

using std::tr1::shared_ptr;

class source;
class path;
class file;

typedef std::vector<shared_ptr<source> > sources;
typedef std::vector<shared_ptr<path> > paths;

#pragma db object
class file
{
public:
    typedef::path path_type;
    typedef::source source_type;
    file (const std::string& name,
          shared_ptr<path_type> pathId,
          shared_ptr<source_type> sourceId)
        : name_ (name), pathId_ (pathId), sourceId_ (sourceId)
  {
  }

  const std::string&
  name () const
  {
    return name_;
  }

  shared_ptr<path_type>
  pathId () const
  {
    return pathId_;
  }

  void
  pathId (shared_ptr<path_type> pathId)
  {
    pathId_ = pathId;
  }

  shared_ptr<source_type>
  sourceId () const
  {
    return sourceId_;
  }

  void
  sourceId (shared_ptr<source_type> sourceId)
  {
    sourceId_ = sourceId;
  }

private:
  friend class odb::access;

  file () {}

  #pragma db id auto
  unsigned long id_;

  std::string name_;

#pragma db not_null
  shared_ptr<path_type> pathId_;

#pragma db not_null
  shared_ptr<source_type> sourceId_;
};


#pragma db view object(file)
struct timeStamp_stat
{
  #pragma db column("count(" + file::id_ + ")")
  std::size_t count;

  #pragma db column("min(" + file::pathId_ + ")")
  unsigned short pathId;

  #pragma db column("max(" + file::sourceId_ + ")")
  unsigned short sourceId;
};

#endif // FILE_HXX
