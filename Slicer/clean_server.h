#pragma once
#include <QtNetwork/qlocalserver.h>
#include <QtNetwork/QLocalSocket>
#include <QtWidgets/QDialog>

/*class Writer
{
    int maxSize;
    char *currentFile;
    int writtenOnCurrentFile;
public:
    Writer();
    void writeToFile(char* data);
};*/

class Clean_Server : public QObject
{
    Q_OBJECT
    QLocalServer *server;
    QLocalSocket *clientConnection;
    //Writer writer;
public:
    Clean_Server();
    void startVLC();

private slots:
    void onNewConnection();
    void onReadyRead();
};

