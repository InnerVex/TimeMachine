#ifndef FILE_HXX
#define FILE_HXX

#include <string>
#include <cstddef> // std::size_t

#include <vector>

#include <odb/core.hxx>
#include <odb/tr1/memory.hxx>

#include <Qt>
#include <QtCore>
#include <QString>

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
    file (const QString name,
          qint32  pathId,
          qint32 sourceId)
        : name_ (name), pathId_ (pathId), sourceId_ (sourceId)
  {
  }
  qint32
  id () const
  {
    return id_;
  }

  void
  id (qint32 id)
  {
    id_ = id;
  }
  const QString&
  name () const
  {
    return name_;
  }

  qint32
  pathId () const
  {
    return pathId_;
  }

  void
  pathId (qint32 pathId)
  {
    pathId_ = pathId;
  }

  qint32
  sourceId () const
  {
    return sourceId_;
  }

  void
  sourceId (qint32 sourceId)
  {
    sourceId_ = sourceId;
  }

private:
  friend class odb::access;

  file () {}

  #pragma db id auto
  qint32 id_;

  const QString name_;

  qint32 pathId_;

  qint32 sourceId_;
};

#endif // FILE_HXX
