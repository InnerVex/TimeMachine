#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::auto_ptr
#include <cstdlib>  // std::exit
#include <iostream>

#include <Qt>
#include <QtCore>
#include <QDir>

#include <odb/database.hxx>

#  include <odb/connection.hxx>
#  include <odb/transaction.hxx>
#  include <odb/schema-catalog.hxx>
#  include <odb/sqlite/database.hxx>

inline std::auto_ptr<odb::database>
create_database ()
{
    using namespace std;
    using namespace odb::core;

    auto_ptr<database> db (
                new odb::sqlite::database ("1.db"
                              , SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));

    {
        connection_ptr c (db->connection ());

        if(QFileInfo("1.db").size() == 0 || !QFileInfo("1.db").exists())
        {
            c->execute ("PRAGMA foreign_keys=OFF");
            transaction t (c->begin ());
            schema_catalog::create_schema (*db,"timeMachine");
            t.commit ();

            c->execute ("PRAGMA foreign_keys=ON");
        }
    }
    return db;
}

#endif // DATABASE_HXX
