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
           qint32 iOffset);

    static void insert(quint32 iDateTime,
                QString iFileName,
                QString iSourceName,
                QString iSourceAdress,
                QString iFilePath,
                qint32 iOffset);
};

#endif // INSERT_H
