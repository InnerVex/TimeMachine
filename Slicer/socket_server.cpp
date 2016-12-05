#include "socket_server.h"
#include "QIODevice"
#include <QtNetwork/qlocalserver.h>
#include <qtnetwork/qlocalsocket.h>
#include <qstring.h>
#include <mutex>
#include <qmutex.h>
#include <qcoreapplication.h>
#include <qdebug.h>
#include <thread>
#include <QDataStream>
#include <QFile>
#include <QtWidgets>

/*void socket_server::incomingConnection(int socketDescriptor) {
    QLocalServer::incomingConnection(socketDescriptor);
    QLocalSocket *socket = nextPendingConnection();
    socket->open(QIODevice::ReadWrite);

    qDebug() << "server connection";

    do {
        const int Timeout = 5 * 1000;
        while (!socket->canReadLine()) {
            if (!socket->waitForReadyRead(Timeout)) {
                return;
            }
        }
        char str[100];
        int n = socket->readLine(str, 100);
    if (n < 0) {
            perror("recv");
            break;
        }
        if (n == 0)
            break;
    qDebug() << "Read" << str;
        if ("exit" == str)
            qApp->quit();

        if (socket->write(str, 100) < 0) {
            perror("send");
            break;
        }
    } while (true);
}*/
QMutex mut2;
const QString socket_name = "my_soket";

/*Socket_Server::Socket_Server()
{
    server = new QLocalServer();
    if (!server->listen(socket_name))
    {
        qDebug()<<"Starting server problem";
    }
    else
    {
        qDebug()<<"Server started"<<server->serverName()<<server->fullServerName();
    }
    writer = server->nextPendingConnection();
    writer->connectToServer();
}*/

int Socket_Server::start_Server()
{    

    QLocalServer server;
    if (!server.listen(socket_name))
    {
        qDebug()<<"Starting server problem";
        return 1;
    }
    else
    {
        //mut2.lock();
        //mut1.lock();
        qDebug()<<"Server started"<<server.serverName()<<server.fullServerName();
        //mut1.unlock();
        //mut2.unlock();
    }

    system("pause");

}

int Socket_Server::write()
{
    //QLocalSocket socket;
    QLocalSocket writer;

    //qDebug()<<"test"<<writer->fullServerName();

    //const char* data = "Hello, socket";
    //qDebug()<<"Amount of data written"<<writer->write(data);
    /*QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    writer->write(block);
    writer->flush();
    writer->disconnectFromServer();*/

    //socket.connectToServer(socket_name);

    writer.connectToServer(socket_name);

    if (writer.waitForConnected())
    {
        qDebug()<<"Writer connected";
    }
    else
    {
        qDebug()<<"Writer is not connected";
        return 1;
    }



    qDebug()<<writer.fullServerName();
    const char* data = "Hello, socket";
    //qDebug()<<"Amount of data written"<<writer.write(data);
    //QDataStream stream(&writer);
    //quint8 in = 'H';
    //stream << in;
    writer.flush();
    //quint8 out = 'A';
    char out[1] = {'A'};

    //qDebug()<<"reading"<<writer.bytesAvailable()<<writer.read(out,1);
    qDebug()<<*out;

    //QFile wr(writer.fullServerName());
    QFile wr("\\.\pipe\my_socket");
    if (!wr.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Failed";
        return 0;
    }
    QDataStream dataStream(&wr);
    dataStream<<"Hi";

    qDebug()<<wr.bytesAvailable();

    //QFile file("D:\\example\\mov.mp4");
    //QFile file(writer.fullServerName());
    QFile file("\\\\.\\pipe\\mysocket");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Failed";
        return 0;
    }
    qDebug()<<file.bytesAvailable();

    /*
    QDataStream dataStream(&file);
    //stream >> out;
    //QDataStream stream2(&writer);
    dataStream >> out;
    qDebug()<<out;*/


    /*const char* data = "Hello, socket";
    qDebug()<<"Amount of data written"<<writer->write(data);

    qDebug()<<writer->flush();
    //QDataStream in(&socket);
    //in<<42;
    //int ch;
    //in >> ch;
    //socket.flush();
    //qDebug()<<ch;

    quint64 limit = 5;
    char data2[limit]="";
    qDebug()<<"test"<<data<<writer->bytesAvailable();
    qDebug()<<writer->read(data2,limit);
    //socket.readData(data,limit);
    qDebug()<<data2;
    */
    system("pause");
}

int Socket_Server::read()
{
    QLocalSocket socket;
    socket.connectToServer(socket_name);
    if (socket.waitForConnected())
    {
        qDebug()<<"Reader connected";
    }
    else
    {
        qDebug()<<"Reader is not connected";
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    quint64 limit = 5;
    char data[limit]="";
    qDebug()<<"test"<<data<<socket.bytesAvailable();
    qDebug()<<socket.read(data,limit);
    //socket.readData(data,limit);
    qDebug()<<data;
}

Server::Server(QWidget *parent)
    : QDialog(parent)
{
    statusLabel = new QLabel;
    statusLabel->setWordWrap(true);
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    server = new QLocalServer(this);
    if (!server->listen("fortune")) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }

    statusLabel->setText(tr("The server is running.\n"
                            "Run the Fortune Client example now."));

    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(server, SIGNAL(newConnection()), this, SLOT(sendFortune()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Fortune Server"));
}

void Server::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconnectFromServer();
}
