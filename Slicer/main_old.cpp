#include <QCoreApplication>
#include "slicer.h"
#include "parser.h"
#include "QDebug"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

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
    slicer.makeMultipleSlices(input, output_location, numberOfSlices);
    //qDebug()<<slicer.getDuration("D:\\Work\\TD\\test\\Projects\\video\\outputFromStream_1.ts");
    //slicer.testFunc();
    //slicer.convertToTS("D:\\example\\outputFromStream.ts","D:\\example\\outputFromStream.ts");
    //slicer.makeSliceFromStreamSmem();


    qDebug()<<"End of main";

    return a.exec();
}
