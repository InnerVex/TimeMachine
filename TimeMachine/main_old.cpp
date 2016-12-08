/*#include <QCoreApplication>
#include "QDebug"
#include <QApplication>
#include <QtCore>

#include "clean_server.h"
#include "client.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Clean_Server server;
    const char* input = "rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
    const char* dst = "D:\\example\\cleanOut";
    int wantedSize = 5000000; //bytes
    server.startRecord(input,dst,wantedSize);

    qDebug()<<"End of main";
    return a.exec();
}
*/
