#ifndef PATH_HXX
#define PATH_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#include <Qt>
#include <QtCore>
#include <QString>

#pragma db object
class path
{
public:
  path (const QString filePath)
      : filePath_ (filePath)
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
  filePath () const
  {
    return filePath_;
  }

private:
  friend class odb::access;

  path () {}

  #pragma db id auto
  qint32 id_;

  const QString filePath_;
};

#endif // PATH_HXX
