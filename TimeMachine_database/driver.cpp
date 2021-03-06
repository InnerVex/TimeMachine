#include <memory>   // std::auto_ptr
#include <iostream>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/connection.hxx>

#include <Qt>
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

void insert (QDateTime iDateTime,
             QString iFileName,
             QString iSourceName,
             QString iSourceAdress,
             QString iFilePath,
             qint32 iOffset);

void select (QDateTime sDateTime);

int main ()
{
  try
{       QDateTime firstDateTime(QDate(2011,1,1),QTime(1,10,00));
        for (qint32 i = 1; i < 100; ++i)
        {
            insert(firstDateTime = firstDateTime.addSecs(1),"frstfl","CAM01","192.186.1.1","/storage/strg",i);
        }
        QDateTime secondDateTime(QDate(2022,2,2),QTime(02,22,00));
        for (qint32 i = 1; i < 100; ++i)
        {
            insert(secondDateTime = secondDateTime.addSecs(1),"scndfile","CAM02","192.222.2.2","/storage/ctrg",i);
        }
        QDateTime thirdDateTime(QDate(2033,3,3),QTime(03,33,00));
        for (qint32 i = 1; i < 100; ++i)
        {
           insert(thirdDateTime = thirdDateTime.addSecs(1),"thrdfile","CAM03","193.333.3.3","/storage/thrtg",i);
        }



        /*
        insert(dateTime.addYears(1),"e2FileName","e2SourceName","e2SourceAdress","e2FilePath",14);
        insert(dateTime.addYears(1).addSecs(1),"e2FileName","e2SourceName","e2SourceAdress","e2FilePath",15);
        insert(dateTime.addYears(1).addSecs(2),"e2FileName","e2SourceName","e2SourceAdress","e2FilePath",16);
        */
        select(firstDateTime.addSecs(-11));
        select(secondDateTime.addSecs(-22));
        select(thirdDateTime.addSecs(-33));
        //select(dateTime.addSecs(2));

        //select(dateTime.addYears(1).addSecs(1));
        //select(dateTime.addYears(1).addSecs(2));
}
  catch (const odb::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}

void insert (QDateTime iDateTime,
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

            cout << iPathId << endl;
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

            cout << iSourceId << endl;
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

void select(QDateTime sDateTime)
//Вывод
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
}
