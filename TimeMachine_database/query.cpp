#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <Qt>
#include <QtCore>
#include <QDir>

#include "database.h" // create_database

#include "source.h"
#include "source-odb.h"

#include "path.h"
#include "path-odb.h"

#include "file.h"
#include "file-odb.h"

#include "timestamp.h"
#include "timestamp-odb.h"

Query::Query()
{

}
