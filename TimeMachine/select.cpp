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

qint32  Select::selectDateTime(QString fileName)
{
    auto_ptr<database> db (create_database ());

    qint32 sDateTime = -1;
    qint32 sFileId = selectFileId(fileName);

    typedef odb::query<timeStamp> query;
    typedef odb::result<timeStamp> result;

    transaction t2 (db->begin ());
    result r (db->query<timeStamp> (query::fileId == sFileId));

    for (result::iterator i (r.begin()); i != r.end(); ++i)
    {
        sDateTime = i->dateTime();
    }
    t2.commit();

    return sDateTime;
}
qint32  Select::selectPreviousDateTime(quint32 sDateTime)
{
  auto_ptr<database> db (create_database ());
    qint32 sNewDateTime = -1;
    qint32 minDateTime = selectMinDateTime();
    if (sDateTime == minDateTime)
    {
        return sDateTime;
    }
    while (sDateTime != sNewDateTime)
    {
        --sDateTime;

        typedef odb::query<timeStamp> query;
        typedef odb::result<timeStamp> result;

        transaction t2 (db->begin ());
        result r (db->query<timeStamp> (query::dateTime == sDateTime));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sNewDateTime = i->dateTime();
        }
        if (sDateTime == minDateTime)
        {
            break;
        }
        t2.commit();
    }
    return sNewDateTime;
}
qint32  Select::selectNextDateTime(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    qint32 sNewDateTime = -1;
    qint32 maxDateTime = selectMaxDateTime();

    if (sDateTime == maxDateTime)
    {
        return -1;
    }

    while (sDateTime != sNewDateTime)
    {
        typedef odb::query<timeStamp> query;
        typedef odb::result<timeStamp> result;

        ++sDateTime;

        transaction t2 (db->begin ());
        result r (db->query<timeStamp> (query::dateTime == sDateTime));


        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sNewDateTime = i->dateTime();
        }
        t2.commit();


        if(sDateTime == maxDateTime)
        {
            break;
        }
    }

    return sNewDateTime;
}

qint32  Select::selectFileId(QString fileName)
{
    auto_ptr<database> db (create_database ());
    qint32 sFileId = -1;
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

        return sFileId;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
qint32  Select::selectPreviousFileId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    sDateTime = selectPreviousDateTime(sDateTime);
    qint32 sFileId = -1;

    typedef odb::query<timeStamp> query;
    typedef odb::result<timeStamp> result;

    transaction t1 (db->begin ());
    result r (db->query<timeStamp> (query::dateTime == sDateTime));

    for (result::iterator i (r.begin()); i != r.end(); ++i)
    {
        sFileId = i->fileId();
    }

    t1.commit();

    return sFileId;

}
qint32  Select::selectNextFileId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    sDateTime = selectNextDateTime(sDateTime);

    qint32 sFileId = -1;

    if (sDateTime == -1)
    {
        return sFileId;
    }

    typedef odb::query<timeStamp> query;
    typedef odb::result<timeStamp> result;

    transaction t1 (db->begin ());
    result r (db->query<timeStamp> (query::dateTime == sDateTime));

    for (result::iterator i (r.begin()); i != r.end(); ++i)
    {
        sFileId = i->fileId();
    }
    t1.commit();

    return sFileId;
}

QString Select::selectFile(quint32 sDateTime)
{
    if (sDateTime <= selectMaxDateTime() && sDateTime >= selectMinDateTime());
    {
        auto_ptr<database> db (create_database ());
        qint32 sFileId = Select::selectPreviousFileId(sDateTime);
        QString sFileName = "";

        typedef odb::query<file> query;
        typedef odb::result<file> result;
        transaction t (db->begin ());
        result r (db->query<file> (query::id == sFileId));

        for (result::iterator i (r.begin()); i != r.end(); ++i)
        {
            sFileName = i->name();
        }
        t.commit();

        return sFileName;
    }
    return "";
}
QString Select::selectNextFile(QString fileName)
{
    QString sFileName = "";

    auto_ptr<database> db (create_database ());

    qint32 sDateTime = selectDateTime(fileName);

    if(sDateTime == -1)
    {
        return sFileName;
    }

    qint32 sFileId = selectNextFileId(sDateTime);

    if(sFileId == -1)
    {
        return sFileName;
    }

    typedef odb::query<file> query;
    typedef odb::result<file> result;
    transaction t (db->begin ());
    result r (db->query<file> (query::id == sFileId));

    for (result::iterator i (r.begin()); i != r.end(); ++i)
    {
        sFileName = i->name();
    }

    t.commit();

    return sFileName;
}

qint32  Select::selectDuration(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());
    qint32 sFileId = selectPreviousFileId(sDateTime);
    qint32 sDuration = -1;
    QString e;

    typedef odb::query<file> query;
    typedef odb::result<file> result;

    transaction t (db->begin ());
    result r (db->query<file> (query::id == sFileId));

    for (result::iterator i (r.begin()); i != r.end(); ++i)
    {
        sDuration = i->duration();

    }
    t.commit();

    return sDuration;
}
qint32  Select::selectOffset(quint32 sDateTime)
{
    qint32 offset = sDateTime - selectPreviousDateTime(sDateTime);

    return offset;
}
float   Select::selectPercentOffset(quint32 sDateTime)
{
    float sOffset = selectOffset(sDateTime);
    float sDuration = selectDuration(sDateTime);
    float percentOffset = sOffset*1000/sDuration;

    return percentOffset;
}

qint32  Select::selectMaxDateTime()
{
    auto_ptr<database> db (create_database ());
    {
        qint32 maxDateTime = -1;

        typedef odb::query<timeStamp> query;

        transaction t (db->begin ());

        timeStamp_stats ts (db->query_value<timeStamp_stats> ());

        maxDateTime = ts.max_dateTime;

        t.commit ();

        return maxDateTime;
    }
}
qint32  Select::selectMinDateTime()
{
    auto_ptr<database> db (create_database ());
    {
        qint32 minDateTime = -1;

        transaction t (db->begin ());

        timeStamp_stats ts (db->query_value<timeStamp_stats> ());

        minDateTime = ts.min_dateTime;

        t.commit ();

        return minDateTime;
    }
}

qint32  Select::selectPathId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sPathId = -1;
    qint32 sFileId =Select::selectPreviousFileId(sDateTime);
    QString sFileName,e;
    try
    {
        typedef odb::query<file> query;
        typedef odb::result<file> result;

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
qint32  Select::selectSourceId(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sSourceId = -1;
    qint32 sFileId = Select::selectPreviousFileId(sDateTime);
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
            e = "Source for time = " + QDateTime::fromTime_t(sDateTime).toString() +" wasn't found";
            throw e;

        }
        return sSourceId;
    }
    catch(QString e)
    {
        cout << e.toStdString().c_str() << endl;
    }
}
QString Select::selectPath(quint32 sDateTime)
{
    auto_ptr<database> db (create_database ());

    qint32 sPathId = Select::selectPathId(sDateTime);
    QString sPath = "";

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

    return sPath;
}
QString Select::selectSourceName(quint32 sDateTime){
    auto_ptr<database> db (create_database ());
    qint32 sSourceId;
    QString sSourceName = "";

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
QString Select::selectSourceAdress(quint32 sDateTime){
    auto_ptr<database> db (create_database ());

    qint32 sSourceId;
    QString sSourceAdress = "";

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
