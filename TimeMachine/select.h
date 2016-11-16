#ifndef SELECT_H
#define SELECT_H

#include <QtCore>

class Select
{
public:
    Select (QDateTime sDateTime);

    //static qint32  select             (QDateTime sDateTime);
    static qint32  selectOffset       (QDateTime sDateTime);
    static qint32  selectPathId       (QDateTime sDateTime);
    static qint32  selectSourceId     (QDateTime sDateTime);
    static qint32  selectFileId       (QDateTime sDateTime);
    static QString selectFile         (QDateTime sDateTime);
    static QString selectPath         (QDateTime sDateTime);
    static QString selectSourceName   (QDateTime sDateTime);
    static QString selectSourceAdress (QDateTime sDateTime);
};

#endif // SELECT_H
