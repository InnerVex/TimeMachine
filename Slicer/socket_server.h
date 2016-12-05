#pragma once
#include <QtNetwork/qlocalserver.h>
#include <QtNetwork/QLocalSocket>
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

#include <QtWidgets/QDialog>

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

