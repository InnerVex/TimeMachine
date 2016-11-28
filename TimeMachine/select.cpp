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

qint32 Select::selectPreviousDateTime(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    QString e;
    qint32 sNewDateTime = -1;
    while (sDateTime != sNewDateTime)
    {
        try
        {
            typedef odb::query<timeStamp> query;
            typedef odb::result<timeStamp> result;

            transaction t2 (db->begin ());
            result r (db->query<timeStamp> (query::dateTime == sDateTime));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sNewDateTime = i->dateTime();
            }
            t2.commit();

            if (sNewDateTime == -1)
            {
                e = "File with time:" + QString::number(sDateTime) + " wasn't found";
                throw e;
            }

            return sNewDateTime;
        }
        catch(QString e)
        {
            //cout << e.toStdString().c_str() << endl;
        }
       --sDateTime;
    }
}
qint32 Select::selectFileId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    QString e;
    sDateTime = selectPreviousDateTime(sDateTime);
    qint32 sFileId = -1;
    try
    {
        typedef odb::query<timeStamp> query;
        typedef odb::result<timeStamp> result;

        transaction t1 (db->begin ());
        result r (db->query<timeStamp> (query::dateTime == sDateTime));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sFileId = i->fileId();
        }
        t1.commit();

        if (sFileId == -1)
        {
            e = "File with time:" + QString::number(sDateTime) + " wasn't found";
            throw e;
        }

        return sFileId;
    }
        catch(QString e)
        {
            cout << e.toStdString().c_str() << endl;
        }

}
qint32 Select::selectDuration(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    qint32 sFileId = selectFileId(sDateTime);
    qint32 sDuration = -1;
    QString e;

    try
    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;

        transaction t (db->begin ());
        result r (db->query<file> (query::id == sFileId));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sDuration = i->duration();

        }
        t.commit();

        if (sDuration == -1)
        {
            e = "Duration of file for time: " + QDateTime::fromTime_t(sDateTime).toString() +"wasn't found";
            throw e;
        }

        return sDuration;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
qint32 Select::selectPathId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sPathId = -1,sFileId;
    QString sFileName,e;
    try
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
        }
        t1.commit();

        if(sPathId == 0)
        {
            e = "Path to file for time = " + QDateTime::fromTime_t(sDateTime).toString() +"wasn't found";
            throw e;
        }

        return sPathId;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
qint32 Select::selectSourceId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sSourceId = -1;
    qint32 sFileId = Select::selectFileId(sDateTime);
    QString e;
    try
    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;

        transaction t1 (db->begin ());
        result r (db->query<file> (query::id == sFileId));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sSourceId = i->sourceId();
        }
        t1.commit();

        if(sSourceId == -1)
        {
            e = "Source for time = " + QDateTime::fromTime_t(sDateTime).toString() +"wasn't found";
            throw e;

        }
        return sSourceId;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
qint32 Select::selectOffset(quint32 sDateTime)
{
    qint32 offset = sDateTime - selectPreviousDateTime(sDateTime);

    return offset;
}
QString Select::selectFile(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    qint32 sFileId;
    QString sFileName = "",e;
    try
    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;
        sFileId = Select::selectFileId(sDateTime);
        transaction t (db->begin ());
        result r (db->query<file> (query::id == sFileId));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sFileName = i->name();
        }
        t.commit();

        if(sFileName == "")
        {
            e = "File for time = " + QDateTime::fromTime_t(sDateTime).toString() +"wasn't found";
            throw e;
        }

        return sFileName;
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

        return sSourceAdress;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
float Select::selectPercentOffset(quint32 sDateTime)
{
    try
    {
        float sOffset = selectOffset(sDateTime);
        float sDuration = selectDuration(sDateTime);
        float percentOffset = sOffset*1000/sDuration;

        if (sOffset > sDuration)
        {
            QString e = "Error in the calculation";
            throw e;
        }

        return percentOffset;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
QString Select::selectNextFile(QString fileName)
{
    auto_ptr<database> db (create_database ());
    qint32 sDateTime = selectNextDateTime(fileName);
    qint32 sFileId = selectFileId(sDateTime);
    QString sFileName = "",e;
    try
    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;
        transaction t (db->begin ());
        result r (db->query<file> (query::id == sFileId));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sFileName = i->name();
        }

        if(sFileName == "")
        {
            e = "File for time = " + QDateTime::fromTime_t(sDateTime).toString() +"wasn't found";
            throw e;
        }

        return sFileName;
        t.commit();
}
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
qint32  Select::selectNextDateTime(QString fileName)
{
    auto_ptr<database> db (create_database ());
    QString e;
    qint32 sDateTime = selectDateTime(fileName)+1;
    qint32 sNewDateTime = -1;
    while (sDateTime != sNewDateTime)
    {
        try
        {
            typedef odb::query<timeStamp> query;
            typedef odb::result<timeStamp> result;

            transaction t2 (db->begin ());
            result r (db->query<timeStamp> (query::dateTime == sDateTime));

            for (result::iterator i (r.begin()); i != r.end(); ++i)
            {
                sNewDateTime = i->dateTime();
            }
            t2.commit();

            if (sNewDateTime == -1)
            {
                // e = "File with time:" + QString::number(sDateTime) + " wasn't found";
                throw e;
            }

            return sNewDateTime;
        }
        catch(QString e)
        {
            //cout << e.toStdString().c_str() << endl;
        }
       ++sDateTime;
    }
}
qint32  Select::selectDateTime(QString fileName)
{
    auto_ptr<database> db (create_database ());
    QString e;
    qint32 sDateTime = -1;
    qint32 sFileId = selectFileId(fileName);
    try
    {
        typedef odb::query<timeStamp> query;
        typedef odb::result<timeStamp> result;

        transaction t2 (db->begin ());
        result r (db->query<timeStamp> (query::fileId == sFileId));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sDateTime = i->dateTime();
        }
        t2.commit();

        if (sDateTime == -1)
        {
            e = "Time for file:" + fileName + " wasn't found";
            throw e;
        }
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
    return sDateTime;
}
qint32  Select::selectFileId(QString fileName)
{
    auto_ptr<database> db (create_database ());
    qint32 sFileId = -1;
    QString e;
    try
    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;

        transaction t (db->begin ());
        result r (db->query<file> (query::name == fileName));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sFileId = i->id();
        }
        t.commit();

        if(sFileId == -1)
        {
            e = "File ID for file = " + fileName +"wasn't found";
            throw e;
        }

        return sFileId;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
