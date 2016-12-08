#include "clean_server.h"
#include "clean_slicer.h"
#include "insert.h"
#include <QFile>
#include <QDataStream>
#include <QDir>


Clean_Server::Clean_Server()
{
    socket_name = "my_socket";
//    QLocalServer::removeServer(socket_name);

    server = new QUdpSocket();
    if ( !server->bind(QHostAddress::LocalHost, 7776) )
    {
        qDebug()<<"Server is not started";
    }
    else
    {
        qDebug()<<"Server started";
    }
    connect(server, SIGNAL(readyRead()), this, SLOT(onReadyRead()));


//    qDebug()<<server->serverName();
}

void Clean_Server::startRecord(const char* input, const char* destination, int wantedSize)
{
    writer = new Writer(destination,wantedSize);
    Clean_Slicer slicer;
//    std::string str = server->fullServerName().toStdString();
//    char *socket;
//    socket = new char[str.length()];
//    strcpy(socket,str.data());
//    qDebug()<<"Socket name:"<<socket;
    QString addr = "127.0.0.1:7776";
    slicer.makeSlicesFromStreamClean(input,addr.toLocal8Bit().data());
}

void Clean_Server::onNewConnection()
{
//    clientConnection = server->nextPendingConnection();
//    QObject::connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
//    QObject::connect(clientConnection, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Clean_Server::onReadyRead()
{
    qDebug()<<"OnReadyRead"<<server->bytesAvailable();
    int limit = 188*100;
    char data[limit];
    int len;

    while (server->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(server->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        server->readDatagram(datagram.data(), datagram.size(),
                             &sender, &senderPort);

        writer->writeToFile(datagram.data(),datagram.size());
    }

//    while (server->bytesAvailable()>0 && (int)(server->bytesAvailable()/limit) > 0)
//    {
//        server->bytesAvailable();
//        len=server->read(data,limit);
//        writer->writeToFile(data,len);
//    }
}

Writer::Writer(const char* destination, int wantedSize)
{
    currNumber = 0;
    maxSize = wantedSize;
    length_of_dst = strlen(destination);
    dst = new char[length_of_dst];
    strcpy(dst,destination);
    currFile = new QFile;
    //time = QDateTime(QDate(2000,01,01),QTime(00,00,00)).toTime_t();

    time = QDateTime::currentDateTime().toTime_t();

    QDir dir;
    qDebug()<<"Creating dir"<<dir.mkdir("example");
}

void Writer::writeToFile(char *data, int len)
{
    //qDebug()<<"Writing";
    if(!currFile->isOpen())
    {
        qDebug()<<"Creating file";
        const char* ending =".ts";
        int rank = 5;
        currName = new char[length_of_dst+strlen(ending) + rank];
        strcpy(currName,dst);
        char str[rank];
        sprintf(str, "%d", currNumber);
        strcat(currName,str);
        strcat(currName,ending);

// Мультиплатформенный путь

        QString tmp(currName);
        currName = QDir::toNativeSeparators(tmp).toLocal8Bit().data();

        currFile = new QFile(currName);
        if (!currFile->open(QIODevice::WriteOnly))
        {
            qDebug()<<"Error at creating file";
            return;
        }
        qDebug()<<"File created"<<currName;

        stream = new QDataStream(currFile);
    }

    if (currFile->size() + len <maxSize)
    {
        stream->writeRawData(data,len);
    }
    else
    {
        currFile->close();

        Clean_Slicer slicer;
        int duration = slicer.getDuration(currName);
        Insert(time,currName,"EXAMPLE_SOURCE_NAME","EXAMPLE_SOURCE_ADRESS","EXAMPLE_FILE_PATH",duration);
        time = time + duration/1000;

        //currFile=nullptr; //sudo collect junk, I dunno how....
        currNumber++;
        writeToFile(data,len);
    }


}
