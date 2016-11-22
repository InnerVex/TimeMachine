#include <QCoreApplication>
#include "slicer.h"
#include "parser.h"
#include "QDebug"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Slicer slicer;    
    const char* input ="http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"; //"D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\output4.ts";
    const char* output ="D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\slices\\slice_5-10.ts";
    const char* outputFromStream = "D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts";
    //slicer.convertToTS(input,output);
    uint start = 0;
    uint stop = 300;
    //slicer.makeSlice(input,start,stop,output);

    //qDebug()<< slicer.getDuration(output);
    slicer.convertToTsFromStream(input,outputFromStream); // there is no stop yet, so turn it off by yourself

    qDebug()<<"End of main";

    return a.exec();
}
