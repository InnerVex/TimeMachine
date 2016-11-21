#include "streamcontroller.h"
#include "playercontroller.h"
#include "player.h"
#include "insert.h"
#include "select.h"
#include "slicer.h"
#include <iostream>
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    {
        Slicer slicer;
        const char* input ="1080p25.ts";
        const char* output1 ="slice1.ts";
        const char* output2 ="slice2.ts";
        //slicer.convertToTS(input,output);
        //input = output;
        //output = "slice.ts";
        uint start = 0;
        uint stop = start + 29;
        slicer.makeSlice(input,start,stop,output1);
        {
            QDateTime DateTime(QDate(2002,01,01),QTime(00,00,01));
            quint32 time = DateTime.toTime_t();
            QString iFileName = output1;
            //QString iFilePath = QFileInfo("slice_0-5.ts").dir();
            QString iSourceName = "CAM01";
            QString iSourceAdress = "1.1.1.1";
            qint32  iOffset = 0;
            for(int i = 0; i < stop; ++i)
            {
                iOffset++;
                Insert::insert(++time,iFileName,iSourceName,iSourceAdress,"home/",iOffset);
            }
        }
        start = start+stop;
        stop = start + 29;
        {
            QDateTime DateTime = DateTime.addSecs(start-29);
            quint32 time = DateTime.toTime_t();
            QString iFileName = output2;
            //QString iFilePath = QFileInfo("slice_5-10.ts").dir();
            QString iSourceName = "CAM01";
            QString iSourceAdress = "1.1.1.1";
            qint32  iOffset = 0;
            for(int i = 0; i < stop; ++i)
            {
                iOffset++;
                Insert::insert(++time,iFileName,iSourceName,iSourceAdress,"home/",iOffset);
            }
        }
        slicer.makeSlice(input,start,stop,output2);
    }

    /*
    qint32 offset = Select::selectOffset(time-66);
    QString path = Select::selectPath(time-66);
    QString sourceAdress = Select::selectSourceAdress(time-66);
    QString sourseName = Select::selectSourceName(time-66);
    QString file = Select::selectFile(time-66);

    std::cout << "Offset:"       << offset
              << " Path:"        << path.toStdString()
              << " SourceAdress:"<< sourceAdress.toStdString()
              << " SourceName:"  << sourseName.toStdString()
              << " File:"        << file.toStdString() << std::endl;
    */
    QApplication a(argc, argv);

    Player player;
    PlayerController *playerController = new PlayerController(&player);
    StreamController *streamController = new StreamController();
    player.show();

    QObject::connect(playerController, &PlayerController::requestToObtainSource,
                    streamController, &StreamController::requestedToObtainSource);
    QObject::connect(playerController, &PlayerController::requestToStream,
                    streamController, &StreamController::requestedToStream);
    QObject::connect(playerController, &PlayerController::requestToPauseStream,
                    streamController, &StreamController::requestedToPauseStream);
    QObject::connect(streamController, &StreamController::signalSourceObtained,
                    playerController, &PlayerController::handleSourceObtained);



    return a.exec();
}
