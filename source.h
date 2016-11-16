#ifndef SOURCE_HXX
#define SOURCE_HXX

#include <string>
#include <cstddef> // std::size_t

#include <odb/core.hxx>

#include <Qt>
#include <QtCore>
#include <QString>

#pragma db object
class source
{
public:
  source (const QString name,
          const QString adress)
      : name_ (name), adress_ (adress)
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

  const QString&
  adress () const
  {
    return adress_;
  }

private:
  friend class odb::access;

  source () {}

  #pragma db id auto
  qint32 id_;

  const QString name_;
  const QString adress_;
};

#endif // SOURCE_HXX
