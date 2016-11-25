#pragma once

#ifndef INSERT_H
#define INSERT_H

#include <QtCore>

class Insert
{
public:
    Insert(quint32 iDateTime,
           QString iFileName,
           QString iSourceName,
           QString iSourceAdress,
           QString iFilePath,
           qint32  iDuration);

    static void insert(quint32 iDateTime,
                QString iFileName,
                QString iSourceName,
                QString iSourceAdress,
                QString iFilePath,
                qint32  iDuration);
};

#endif // INSERT_H
