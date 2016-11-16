#ifndef TIMESTAMP_HXX
#define TIMESTAMP_HXX

#include <string>
#include <cstddef> // std::size_t

#include <Qt>
#include <QtCore>
#include <QString>

#include <odb/core.hxx>
#include <odb/tr1/memory.hxx>


#include "file.h"
using std::tr1::shared_ptr;

class file;
class timeStamp;

#pragma db object
class timeStamp
{
public:
    timeStamp (qint32 fileId,
               QDateTime dateTime,
               qint32 offset)
        : fileId_ (fileId), dateTime_ (dateTime), offset_ (offset)
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
  qint32
  fileId () const
  {
    return fileId_;
  }
  void
  fileId (qint32 fileId)
  {
    fileId_ = fileId;
  }

  QDateTime
  dateTime () const
  {
    return dateTime_;
  }

  qint32
  offset () const
  {
    return offset_;
  }

  void
  offset (qint32 offset)
  {
    offset_ = offset;
  }

private:
  friend class odb::access;

  timeStamp () {}

  #pragma db id auto
  qint32 id_;

  qint32 fileId_;

  QDateTime dateTime_;

  qint32 offset_;
};
#endif // TIMESTAMP_HXX
