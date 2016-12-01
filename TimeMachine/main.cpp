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
        /*
        Slicer slicer;
        const char* input ="rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
        int numberOfSlices = 3;
        slicer.makeMultipleSlices(input, numberOfSlices);
        /*
        {
            QDateTime DateTime(QDate(2002,01,01),QTime(00,00,01));
            quint32 time = DateTime.toTime_t();
            QString iFileName = output;
            //QString iFilePath = QFileInfo("slice_0-5.ts").dir();
            QString iSourceName = "CAM01";
            QString iSourceAdress = "1.1.1.1";
            quint32 iDuration = 333333;
            for(int i = 0; i < stop; ++i)
            {
                iOffset++;
                Insert::insert(++time,iFileName,iSourceName,iSourceAdress,"home/",iDuration);
            }
        }
        */
    }
    //Тест ввода-вывода и бд
    {
        QDateTime InsertDateTime(QDate(2000,01,01),QTime(00,00,00));
        {
            InsertDateTime;
            qint32 time1 = InsertDateTime.toTime_t();
            QString iFileName = "outputFromStream_0.ts";
            //QString iFilePath = QFileInfo("slice_0-5.ts").dir();
            QString iSourceName = "CAM_01";
            QString iSourceAdress = "1.1.1.1";
            quint32 iDuration = 600000;
            Insert::insert(time1,iFileName,iSourceName,iSourceAdress,"EXAMPLE_FILE_PATH",iDuration);
        }
        {
            qint32 time2 = InsertDateTime.addMSecs(600000).toTime_t();
            QString iFileName = "outputFromStream_1.ts";
            //QString iFilePath = QFileInfo("slice_0-5.ts").dir();
            QString iSourceName = "CAM_01";
            QString iSourceAdress = "1.1.1.1";
            quint32 iDuration = 600000;
            Insert::insert(time2,iFileName,iSourceName,iSourceAdress,"EXAMPLE_FILE_PATH",iDuration);
        }

        QDateTime SelectDateTime(QDate(2000,01,01),QTime(00,05,00));
        qint32 time = SelectDateTime.toTime_t();
        std::cout << "DateTime:"     << Select::selectPreviousDateTime(time)
                  << " Duration:"    << Select::selectDuration(time)
                  << " Path:"        << Select::selectPath(time).toStdString()
                  << " SourceAdress:"<< Select::selectSourceAdress(time).toStdString()
                  << " SourceName:"  << Select::selectSourceName(time).toStdString()
                  << " File:"        << Select::selectFile(time).toStdString()
                  << " Offset:"      << Select::selectOffset(time)
                  << " PercenOffset:"<< Select::selectPercentOffset(time)
                  << " NextFile:"    << Select::selectNextFile(Select::selectFile(time)).toStdString() << std::endl;
    }

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
