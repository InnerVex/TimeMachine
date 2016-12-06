#pragma once
#include <QtNetwork/qlocalserver.h>
#include <QtNetwork/QLocalSocket>
#include <QtWidgets/QDialog>
class Socket_Server
{

    //QLocalServer *server;
    //QLocalSocket *writer;

public:
    Socket_Server();
    //void incomingConnection(int socketDescriptor);
    static int start_Server();
    static int write();
    static int read();

};

class New_Socket_Server : public QObject
{
    Q_OBJECT
    QLocalServer *server;
    QLocalSocket *socket;
    QString currentFortune;
    quint16 blockSize;
    QLocalSocket *clientConnection;
    //QLocalSocket *writer;
    bool work = true;
public:
    New_Socket_Server();
    void test();
    //int sendMsg();
//private slots:
    int recieveMsg();
private slots:
    int sendMsg();
    void onReadyRead();
};



class QLabel;
class QPushButton;
class QLocalServer;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);

private slots:
    void sendFortune();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QLocalServer *server;
    QStringList fortunes;
};

