#include "clean_server.h"
#include "clean_slicer.h"
//#include "insert.h"
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

void Clean_Server::startRecord(const char* input, const char* destination, int wantedSize)
{
    writer = new Writer(destination,wantedSize);
    Clean_Slicer slicer;
    std::string str = server->fullServerName().toStdString();
    char *socket;
    socket = new char[str.length()];
    strcpy(socket,str.data());
    qDebug()<<"Socket name:"<<socket;
    slicer.makeSlicesFromStreamClean(input,socket);
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
    int len;

    while (clientConnection->bytesAvailable()>0 && (int)(clientConnection->bytesAvailable()/limit) > 0)
    {
        clientConnection->bytesAvailable();
        len=clientConnection->read(data,limit);
        writer->writeToFile(data,len);
    }
}

Writer::Writer(const char* destination, int wantedSize)
{
    currNumber = 0;
    maxSize = wantedSize;
    length_of_dst = strlen(destination);
    dst = new char[length_of_dst];
    strcpy(dst,destination);
    currFile = new QFile;
    time = QDateTime(QDate(2000,01,01),QTime(00,00,00)).toTime_t();
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
