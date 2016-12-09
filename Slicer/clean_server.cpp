#include "clean_server.h"
#include "clean_slicer.h"
//#include "insert.h"
#include <QFile>
#include <QDataStream>
#include <QTime>
#include <QDir>


//const QString socket_name = "my_socket" + qrand();
Clean_Server::Clean_Server()
{
    QTime tmp_time = QTime::currentTime();
    qsrand((uint)tmp_time.msec());

    char str[5];
    sprintf(str, "%d", qrand());
    socket_name = "my_socket";
    socket_name.append(str);
    qDebug()<<socket_name;

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

Writer::Writer(const char* destination_folder, int wantedSize)
{
    maxSize = wantedSize;
    length_of_dst = strlen(destination_folder);
    dst = new char[length_of_dst];
    strcpy(dst,destination_folder);
    currFile = new QFile;
    QDir dir;
    dir.mkdir(destination_folder);
    time=QDateTime::currentDateTime().toTime_t();
}

void Writer::writeToFile(char *data, int len)
{
    //qDebug()<<"Writing";
    if(!currFile->isOpen())
    {
        qDebug()<<"Creating file";
        const char* first_part_of_name = "out";
        const char* ending =".ts";
        int rank = 10;
        currName = new char[length_of_dst+2+strlen(first_part_of_name)+strlen(ending) + rank];
        strcpy(currName,dst);
        strcat(currName,"\\");
        strcat(currName,first_part_of_name);
        char str[rank];
        sprintf(str, "%d", QDateTime::currentDateTimeUtc().toTime_t());
        strcat(currName,str);
        strcat(currName,ending);

        QString native_path(currName);
        strcpy(currName,QDir::toNativeSeparators(native_path).toLocal8Bit().data());
        qDebug()<<currName;
        currFile = new QFile(currName);
        if (!currFile->open(QIODevice::WriteOnly))
        {
            qDebug()<<"Error at creating file";
            return;
        }
        qDebug()<<"File created"<<currName<<QDir::toNativeSeparators(native_path);

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
        qDebug()<<currName<<time<<duration;
        //Insert(time,currName,"EXAMPLE_SOURCE_NAME","EXAMPLE_SOURCE_ADRESS","EXAMPLE_FILE_PATH",duration);
        time = time + duration/1000;

        //currFile=nullptr; //sudo collect junk, I dunno how....
        writeToFile(data,len); //reccursion
    }


}
