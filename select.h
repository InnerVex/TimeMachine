#ifndef SELECT_H
#define SELECT_H

#include <QtCore>

class Select
{
public:
    Select (quint32 sDateTime);

    //static qint32  select             (QDateTime sDateTime);
    static qint32  selectOffset       (quint32 sDateTime);
    static qint32  selectPathId       (quint32 sDateTime);
    static qint32  selectSourceId     (quint32 sDateTime);
    static qint32  selectFileId       (quint32 sDateTime);
    static QString selectFile         (quint32 sDateTime);
    static QString selectPath         (quint32 sDateTime);
    static QString selectSourceName   (quint32 sDateTime);
    static QString selectSourceAdress (quint32 sDateTime);
};

#endif // SELECT_H
