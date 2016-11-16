#pragma once

#ifndef INSERT_H
#define INSERT_H

#include <QtCore>

class Insert
{
public:
    Insert(QDateTime iDateTime,
           QString iFileName,
           QString iSourceName,
           QString iSourceAdress,
           QString iFilePath,
           qint32 iOffset);

    static void insert(QDateTime iDateTime,
                QString iFileName,
                QString iSourceName,
                QString iSourceAdress,
                QString iFilePath,
                qint32 iOffset);
};

#endif // INSERT_H
