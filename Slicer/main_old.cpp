#include <QCoreApplication>
//#include "slicer.h"
//#include "parser.h"
#include "QDebug"
#include <QApplication>
#include <QtCore>
//#include "socket_server.h"
#include "clean_server.h"
#include "client.h"
#include <QThread>
//#include <thread>
int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    QApplication a(argc, argv);
    Clean_Server server;
    server.startVLC();
     qDebug()<<"test";
    //QApplication app(argc, argv);
    //My_Client client;

    //return app.exec();
    //server.sendMsg();
    //qDebug()<<server.recieveMsg();
    //My_Client client;
    //server.test();
    //QThread for_vlc;
    //std::thread for_vlc(New_Socket_Server::test,server);

    //for_vlc.join();
    //qDebug()<<server.recieveMsg();

    /*QApplication app(argc, argv);
    Server server;
    server.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QApplication a(argc, argv);
    Client client;
    client.show();
    return a.exec();


    return app.exec();*/

    /*
    Slicer slicer;    
    const char* input ="rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp"; //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"; //"D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\output4.ts";
    const char* output ="D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\slices\\slice_5-10.ts";
    const char* outputFromStream = "D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts";
    //slicer.convertToTS(input,output);
    uint start = 0;
    uint stop = 300;
    //slicer.makeSlice(input,start,stop,output);

    //qDebug()<< slicer.getDuration(output);
    //slicer.convertToTsFromStream(input,outputFromStream); // there is no stop yet, so turn it off by yourself
    //slicer.makeSliceFromStream();
    int numberOfSlices = 3;
    const char* output_location = "D:\\example";
    //slicer.makeMultipleSlices(input, output_location, numberOfSlices);
    //slicer.makeSlicesUsingSocket(input, output_location, numberOfSlices);
    //qDebug()<<slicer.getDuration("D:\\Work\\TD\\test\\Projects\\video\\outputFromStream_1.ts");
    //slicer.testFunc();
    //slicer.convertToTS("D:\\example\\outputFromStream.ts","D:\\example\\outputFromStream.ts");
    //slicer.makeSliceFromStreamSmem();

    */



    qDebug()<<"End of main";

    return a.exec();
}
