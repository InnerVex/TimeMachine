#ifndef TIMESTAMP_HXX
#define TIMESTAMP_HXX

#include <string>
#include <cstddef> // std::size_t

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
    typedef::file file_id;
    timeStamp (shared_ptr<file_id> fileId,
               const std::string& dateTime,
               unsigned short offset)
        : fileId_ (fileId), dateTime_ (dateTime), offset_ (offset)
  {
  }

  void
  fileId (shared_ptr<file_id> fileId)
  {
    fileId_ = fileId;
  }

  const std::string&
  dateTime () const
  {
    return dateTime_;
  }

  unsigned short
  offset () const
  {
    return offset_;
  }

  void
  offset (unsigned short offset)
  {
    offset_ = offset;
  }

private:
  friend class odb::access;

  timeStamp () {}

  #pragma db id auto
  unsigned long id_;

  #pragma db not_null
  shared_ptr<file_id> fileId_;

  std::string dateTime_;

  #pragma db not_null
  unsigned short offset_;
};


#pragma db view object(timeStamp)
struct file_stat
{
  #pragma db column("count(" + timeStamp::id_ + ")")
  std::size_t count;

  #pragma db column("min(" + timeStamp::fileId_ + ")")
  unsigned short fileId;

  #pragma db column("max(" + timeStamp::dateTime_ + ")")
  const std::string dateTime;

  #pragma db column("max(" + timeStamp::offset_ + ")")
  unsigned short offset;

};

#endif // TIMESTAMP_HXX
