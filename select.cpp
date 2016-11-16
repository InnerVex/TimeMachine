#include "select.h"

#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/connection.hxx>

#include <QtCore>

#include "database.h" // create_database

#include "source.h"
#include "source-odb.h"

#include "path.h"
#include "path-odb.h"

#include "file.h"
#include "file-odb.h"

#include "timestamp.h"
#include "timestamp-odb.h"

using namespace std;
using namespace odb::core;

Select::Select(quint32 sDateTime)
{
    //select(sDateTime);
    selectFile(sDateTime);
    selectPath(sDateTime);
    selectSourceName(sDateTime);
    selectSourceAdress(sDateTime);
    selectOffset(sDateTime);
}
/*
void Select::select(QDateTime sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sFileId = 0,sPathId = 0,sSourceId = 0,sOffset = 0;
    QString sPath,sFileName,sSourceAdress,sSourceName,e;
    try
    {

        {
            typedef odb::query<timeStamp> query;
            typedef odb::result<timeStamp> result;

            transaction t (db->begin ());
            result r (db->query<timeStamp> (query::dateTime == sDateTime));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sFileId = i->fileId();
                sOffset = i->offset();
            }
            t.commit();

            if (sFileId == 0)
            {
                e = "File with time:" + sDateTime.toString(Qt::ISODate) + " does't found";
                throw e;
            }
            if (sOffset == 0)
            {
                e = "Offset in file for time" +sDateTime.toString() +"does't found";
            }
        }
        {
            typedef odb::query<file> query;
            typedef odb::result<file> result;

            transaction t (db->begin ());
            result r (db->query<file> (query::id == sFileId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sPathId = i->pathId();
                sSourceId = i->sourceId();
                sFileName = i->name();

                if(sPathId == 0)
                {
                    e = "Path to file for time = " +sDateTime.toString() +" does't found";
                    throw e;
                }

                if(sSourceId == 0)
                {
                    e = "Source for time = " + sDateTime.toString() +" does't found";
                }
            }
            t.commit();
        }
        {
            typedef odb::query<source> query;
            typedef odb::result<source> result;

            transaction t (db->begin ());
            result r (db->query<source> (query::id == sSourceId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sSourceName = i->name();
                sSourceAdress = i->adress();

            }
            t.commit();
        }
        {
            typedef odb::query<path> query;
            typedef odb::result<path> result;

            transaction t (db->begin ());
            result r (db->query<path> (query::id == sPathId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sPath = i->filePath();
            }
            t.commit();
        }
        cout << "OUTPUT : "
             << "Path: " <<  sPath.toStdString().c_str()
             << " File: " << sFileName.toStdString().c_str()
             << " Offset: " << sOffset << endl;

}
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}*/
qint32 Select::selectFileId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    QString e;
    qint32 sFileId = -1,sOffset = -1;
    try
    {

        {
            typedef odb::query<timeStamp> query;
            typedef odb::result<timeStamp> result;

            transaction t1 (db->begin ());
            result r (db->query<timeStamp> (query::dateTime == sDateTime));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sFileId = i->fileId();
                sOffset = i->offset();
            }
            t1.commit();

            if (sFileId == -1)
            {
                e = "File with time:" + QString::number(sDateTime) + " does't found";
                throw e;
            }
            if (sOffset == -1)
            {
                e = "Offset in file for time" +QString::number(sDateTime) +"does't found";
            }

            return sFileId;
        }
    }
        catch(QString e)
        {
            cout << e.toStdString().c_str() << endl;
        }

}
qint32 Select::selectOffset(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sFileId = -1,sOffset = -1;
    QString e;

    try
    {
        typedef odb::query<timeStamp> query;
        typedef odb::result<timeStamp> result;

        transaction t (db->begin ());
        result r (db->query<timeStamp> (query::dateTime == sDateTime));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sFileId = i->fileId();
            sOffset = i->offset();
        }
        t.commit();

        if (sFileId == -1)
        {
            e = "File with time:" + QString::number(sDateTime) + " does't found";
            throw e;
        }
        if (sOffset == -1)
        {
            e = "Offset in file for time" + QString::number(sDateTime) +"does't found";
        }

        return sOffset;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }

}
qint32 Select::selectPathId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sPathId = -1,sSourceId = -1,sFileId;
    QString sFileName,e;
    try
    {
        {
            typedef odb::query<file> query;
            typedef odb::result<file> result;
            sFileId = Select::selectFileId(sDateTime);
            transaction t1 (db->begin ());
            result r (db->query<file> (query::id == sFileId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sPathId = i->pathId();
                sFileName = i->name();

                if(sPathId == 0)
                {
                    e = "Path to file for time = " + QString::number(sDateTime) +" does't found";
                    throw e;
                }

                return sPathId;
            }
            t1.commit();
        }
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
qint32 Select::selectSourceId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sSourceId = -1,sFileId;
    QString e;
    try
    {
        {
            typedef odb::query<file> query;
            typedef odb::result<file> result;
            sFileId = Select::selectFileId(sDateTime);
            transaction t1 (db->begin ());
            result r (db->query<file> (query::id == sFileId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sSourceId = i->sourceId();

                if(sSourceId == 0)
                {
                    e = "Source for time = " + QString::number(sDateTime) +" does't found";
                    throw e;

                }

            }
            t1.commit();
        }
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }}
QString Select::selectFile(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    qint32 sFileId;
    QString sFileName = "",e;
    try
    {
        {
            typedef odb::query<file> query;
            typedef odb::result<file> result;
            sFileId = Select::selectFileId(sDateTime);
            transaction t (db->begin ());
            result r (db->query<file> (query::id == sFileId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sFileName = i->name();

                if(sFileName == "")
                {
                    e = "File for time = " + QString::number(sDateTime) + " does't found";
                    throw e;
                }

                return sFileName;
            }
            t.commit();
        }
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
QString Select::selectPath(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    qint32 sPathId;
    QString sPath;
    try
    {
        {
            typedef odb::query<path> query;
            typedef odb::result<path> result;
            sPathId = Select::selectPathId(sDateTime);
            transaction t (db->begin ());
            result r (db->query<path> (query::id == sPathId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sPath = i->filePath();
            }
            t.commit();
        }

        return sPath;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
QString Select::selectSourceName(quint32 sDateTime){
    auto_ptr<database> db (create_database ());
    qint32 sSourceId;
    QString sSourceName;
    try
    {
        {
            typedef odb::query<source> query;
            typedef odb::result<source> result;
            sSourceId = Select::selectSourceId(sDateTime);
            transaction t (db->begin ());
            result r (db->query<source> (query::id == sSourceId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sSourceName = i->name();

            }
            t.commit();
        }

        return sSourceName;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
QString Select::selectSourceAdress(quint32 sDateTime){
    auto_ptr<database> db (create_database ());
    qint32 sSourceId;
    QString sSourceAdress;
    try
    {
        {
            typedef odb::query<source> query;
            typedef odb::result<source> result;
            sSourceId = Select::selectSourceId(sDateTime);
            transaction t (db->begin ());
            result r (db->query<source> (query::id == sSourceId));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sSourceAdress = i->adress();
            }
            t.commit();
        }

        return sSourceAdress;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
