#include "clean_server.h"
#include "clean_slicer.h"
#include <QFile>
#include <QDataStream>


const QString socket_name = "my_socket";
Clean_Server::Clean_Server()
{
    server = new QLocalServer();
    if (!server->listen(socket_name))
    {
        qDebug()<<"Server is not started";
    }
    else
    {
        qDebug()<<"Server started";
    }
    server->connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    qDebug()<<server->serverName();
}

void Clean_Server::startVLC()
{
    Clean_Slicer slicer;
    const char* input ="rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
    std::string str = server->fullServerName().toStdString();
    char *socket;
    socket = new char[str.length()];
    strcpy(socket,str.data());
    qDebug()<<socket;
    slicer.makeSlicesFromStreamClean(input,socket,30);
}

void Clean_Server::onNewConnection()
{
    clientConnection = server->nextPendingConnection();
    QObject::connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
    QObject::connect(clientConnection, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Clean_Server::onReadyRead()
{
    //qDebug()<<"OnReadyRead"<<clientConnection->bytesAvailable();
    int limit = 188*100;
    char data[limit];

    while (clientConnection->bytesAvailable()>0 && (int)(clientConnection->bytesAvailable()/limit) > 0)
    {
        qDebug()<<clientConnection->bytesAvailable()<<"left";
        qDebug()<<clientConnection->read(data,limit);
        //writer.writeToFile(data);
        //qDebug()<<"Recieved "<<ch;
    }
}
/*
Writer::Writer()
{
    maxSize = 188 * 100000;
    //currentFile = new char[];
    writtenOnCurrentFile = 0;
}

void Writer::writeToFile(char *data)
{
    QFile file(input);
    if (!file.open(QIODevice::WriteOnly))
        return 0;

    QDataStream dataStream(&file);
    dataStream.
}*/
