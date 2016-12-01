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
               quint32 dateTime)
        : fileId_ (fileId), dateTime_ (dateTime)
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

  quint32
  dateTime () const
  {
    return dateTime_;
  }

private:
  friend class odb::access;

  timeStamp () {}

  #pragma db id auto
  qint32 id_;

  qint32 fileId_;
  #pragma db unique
  quint32 dateTime_;
};
#endif // TIMESTAMP_HXX
