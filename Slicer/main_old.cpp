#include <QCoreApplication>
#include "slicer.h"
#include "parser.h"
#include "QDebug"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Slicer slicer;    
    const char* input ="D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\output4.ts";
    const char* output ="D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\slices\\slice_5-10.ts";
    //slicer.convertToTS(input,output);
    uint start = 0;
    uint stop = 300;
    slicer.makeSlice(input,start,stop,output);

    qDebug()<<"End of main";

    return a.exec();
}
