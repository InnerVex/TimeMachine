#pragma once
#include <QtNetwork/qlocalserver.h>
#include <QtNetwork/QLocalSocket>
#include <QtWidgets/QDialog>
#include <QFile>
#include <QDataStream>
#include <QString>

class Writer
{
    int maxSize;
    QFile *currFile;
    QDataStream *stream;
    char * dst;
    qint32 time;
    char *currName;
    int length_of_dst;
public:
    Writer(const char* destination_folder, int wantedSize = 188 * 10000);
    void writeToFile(char* data,int len);
};

class Clean_Server : public QObject
{
    Q_OBJECT
    QLocalServer *server;
    QLocalSocket *clientConnection;
    Writer *writer;
    QString socket_name;
public:
    Clean_Server();
    void startRecord(const char* input, const char* destination, int wantedSize = 188 * 10000);

private slots:
    void onNewConnection();
    void onReadyRead();
};

