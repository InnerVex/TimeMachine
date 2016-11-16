#include "insert.h"

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

Insert::Insert(QDateTime iDateTime,
               QString iFileName,
               QString iSourceName,
               QString iSourceAdress,
               QString iFilePath,
               qint32 iOffset)
{
    insert(iDateTime,iFileName,iSourceName,iSourceAdress,iFilePath,iOffset);
}

void Insert::insert(QDateTime iDateTime,
                    QString iFileName,
                    QString iSourceName,
                    QString iSourceAdress,
                    QString iFilePath,
                    qint32 iOffset)
{
    auto_ptr<database> db (create_database ());
    //Ввод
    qint32 iPathId = 0,
            iSourceId = 0,
            iFileId = 0,
            iTimeStampId = 0,
            pathId,sourceId,fileId,timeStampId;
    try
    {
        {
            {
                typedef odb::query<path> query;
                typedef odb::result<path> result;

                transaction t (db->begin ());

                result r (db->query<path> (query::filePath == iFilePath));

                for (result::iterator i (r.begin()); i != r.end(); ++i)
                {
                    iPathId = i->id();
                }
                t.commit();

                if (iPathId != 0)
                {
                    throw iFilePath;
                }
            }

            path ePath (iFilePath);
            transaction t (db->begin ());
            pathId = db ->persist(ePath);
            t.commit();
        }
    }
    catch(QString iFilePath)
    {
        cout << "Path: " << iFilePath.toStdString().c_str() << " already exists. " << endl;
    }

    try
    {
        {
            {
                typedef odb::query<source> query;
                typedef odb::result<source> result;

                transaction t (db->begin ());

                result r (db->query<source> (query::adress == iSourceAdress));

                for (result::iterator i (r.begin()); i != r.end(); ++i)
                {
                    iSourceId = i->id();
                }
                t.commit();

                if (iSourceId != 0)
                {
                    throw iSourceAdress;
                }
            }

            source eSource (iSourceName,iSourceAdress);
            transaction t (db->begin ());
            sourceId = db ->persist(eSource);
            t.commit();
        }
    }
    catch(QString iSourceAdress)
    {
        cout << "Source adress: " << iSourceAdress.toStdString().c_str() << " already exists. " << endl;
    }
    try
    {
        {
            typedef odb::query<file> query;
            typedef odb::result<file> result;

            transaction t (db->begin ());

            result r (db->query<file> (query::name == iFileName));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                iFileId = i->id();
            }
            t.commit();

            if (iFileId != 0)
            {
                throw iFileName;
            }
        }
        {
            typedef odb::query<path> query;
            typedef odb::result<path> result;

            transaction t (db->begin ());

            result r (db->query<path> (query::filePath == iFilePath));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                iPathId = i->id();
            }
            t.commit();
        }
        {
            typedef odb::query<source> query;
            typedef odb::result<source> result;

            transaction t (db->begin ());

            result r (db->query<source> (query::name == iSourceName));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                iSourceId = i->id();
            }
            t.commit();
        }
        file eFile (iFileName,iPathId,iSourceId);
        transaction t (db->begin ());
        fileId = db ->persist(eFile);
        t.commit();
    }

    catch(QString iFileName)
    {
        cout << "File: " << iFileName.toStdString().c_str() << " already exists. " << endl;
    }

    try
    {
        {
            typedef odb::query<file> query;
            typedef odb::result<file> result;

            transaction t (db->begin ());

            result r (db->query<file> (query::name == iFileName));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                iFileId = i->id();
            }
            t.commit();
        }
        {
            typedef odb::query<timeStamp> query;
            typedef odb::result<timeStamp> result;

            transaction t (db->begin ());

            result r (db->query<timeStamp> (query::fileId == iFileId && query::offset == iOffset));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                iTimeStampId = i->id();

            }
            t.commit();

            if (iTimeStampId != 0)
            {
                throw iFileName,iOffset;
            }
        }

        timeStamp eTimeStamp (iFileId,iDateTime,iOffset);
        transaction t (db->begin ());
        timeStampId = db ->persist(eTimeStamp);
        t.commit();
    }
    catch(QString iFileName)
    {
        cout << "Offset: " << iOffset << " in " << iFileName.toStdString().c_str() << " already exists. " << endl;
    }
}
