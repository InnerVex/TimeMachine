#pragma once
#include <QtNetwork/qlocalserver.h>
#include <QtNetwork/QLocalSocket>
#include <QtWidgets/QDialog>
#include <QFile>
#include <QDataStream>

class Writer
{
    int maxSize;
    QFile *currFile;
    QDataStream *stream;
    char * dst;
    int currNumber;
public:
    Writer(const char* destination, int wantedSize = 188 * 10000);
    void writeToFile(char* data,int len);
};

class Clean_Server : public QObject
{
    Q_OBJECT
    QLocalServer *server;
    QLocalSocket *clientConnection;
    Writer *writer;
public:
    Clean_Server();
    void startRecord(const char* input, const char* destination, int wantedSize = 188 * 10000);

private slots:
    void onNewConnection();
    void onReadyRead();
};

