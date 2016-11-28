#ifndef SELECT_H
#define SELECT_H

#include <QtCore>

class Select
{
public:
    static qint32  selectPreviousDateTime(quint32 sDateTime);
    static qint32  selectDuration        (quint32 sDateTime);
    static QString selectFile            (quint32 sDateTime);
    static QString selectPath            (quint32 sDateTime);
    static QString selectSourceName      (quint32 sDateTime);
    static QString selectSourceAdress    (quint32 sDateTime);
    static qint32  selectOffset          (quint32 sDateTime);
    static float   selectPercentOffset   (quint32 sDateTime);
    static QString selectNextFile        (QString fileName);


private:
    static qint32  selectNextDateTime   (QString fileName);
    static qint32  selectDateTime       (QString fileName);
    static qint32  selectFileId         (QString fileName);
    static qint32  selectPathId         (quint32 sDateTime);
    static qint32  selectSourceId       (quint32 sDateTime);
    static qint32  selectFileId         (quint32 sDateTime);

};

#endif // SELECT_H
