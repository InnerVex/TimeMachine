#include <iostream>
#include <QApplication>
#include <QObject>

#include "streamcontroller.h"
#include "playercontroller.h"
#include "player.h"
#include "insert.h"
#include "select.h"
#include "slicer.h"
#include "clean_server.h"


int main(int argc, char *argv[])
{

    /*
    {

        Slicer slicer;
        const char* input ="rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
        int numberOfSlices = 3;
        slicer.makeMultipleSlices(input, numberOfSlices);

    }

    {
    /*
    QDateTime SelectDateTime(QDate(2000,01,01),QTime(00,00,10));
    qint32 time = SelectDateTime.toTime_t();
    qint32 previousDateTime = Select::selectPreviousDateTime(time);
    qint32 duration = Select::selectDuration(time);
    QString path = Select::selectPath(time);
    QString sourceadress = Select::selectSourceAdress(time);
    QString sourceName = Select::selectSourceName(time);

    QString file = Select::selectFile(946659600);
    qint32 time = 946659600 + 10;
    file = Select::selectFile(time);
    time = 946659657;
    file = Select::selectFile(time);
    time = time + 10;
    file = Select::selectFile(time);

    qint32 offset = Select::selectOffset(time);
    float percentOffset = Select::selectPercentOffset(time);
    QString nextFile = Select::selectNextFile("qweroutputFromStream_0.ts");
    qint32 max = Select::selectMaxDateTime();
    qint32 min = Select::selectMinDateTime();

    std::cout << "DateTime:"     << Select::selectPreviousDateTime(time)
              << " Duration:"    << Select::selectDuration(time)
              << " Path:"        << Select::selectPath(time).toStdString()
              << " SourceAdress:"<< Select::selectSourceAdress(time).toStdString()
              << " SourceName:"  << Select::selectSourceName(time).toStdString()
              << " File:"        << Select::selectFile(time).toStdString()
              << " Offset:"      << Select::selectOffset(time)
              << " PercenOffset:"<< Select::selectPercentOffset(time)
              << " NextFile:"    << Select::selectNextFile(Select::selectFile(time)).toStdString()
                                 << std::endl;
    }
    */
    QApplication a(argc, argv);

    Clean_Server server;
    const char* input = "rtsp://10.0.0.197:8554/CH001.sdp";
    int wantedSize = 5000000; //bytes, sometimes vlc can't determine duration if file shorter than 6 sec, so probably should make file bigger
    server.startRecord(input,dst,wantedSize);

    Player player;
    player.show();

    PlayerController *playerController = new PlayerController(&player);
    StreamController *streamController = new StreamController(&player);

    //Сигналы от Проигрывателя и GUI к Дешинковщику
    QObject::connect(playerController, &PlayerController::requestToObtainSource,
                    streamController, &StreamController::requestedToObtainSource);
    QObject::connect(playerController, &PlayerController::requestToStreamArchive,
                    streamController, &StreamController::requestedToStreamArchive);
    QObject::connect(playerController, &PlayerController::requestToStreamRealTime,
                    streamController, &StreamController::requestedToStreamRealTime);
    QObject::connect(playerController, &PlayerController::requestToPauseArchive,
                    streamController, &StreamController::requestedToPauseArchive);
    QObject::connect(playerController, &PlayerController::requestToPauseRealTime,
                    streamController, &StreamController::requestedToPauseRealTime);
    QObject::connect(playerController, &PlayerController::requestToSpeedUp,
                    streamController, &StreamController::requestedToSpeedUp);
    QObject::connect(playerController, &PlayerController::requestToSpeedDown,
                    streamController, &StreamController::requestedToSpeedDown);
    QObject::connect(playerController, &PlayerController::requestToStop,
                    streamController, &StreamController::requestedToStop);

    //Сигналы от Дешинковщика к Проигрывателю и GUI
    QObject::connect(streamController, &StreamController::signalSourceObtained,
                    playerController, &PlayerController::handleSourceObtained);
    QObject::connect(streamController, &StreamController::signalStreamStarted,
                    playerController, &PlayerController::attemptToPlayStream);
    QObject::connect(streamController, &StreamController::signalTimerStart,
                    playerController, &PlayerController::startPlayTimer);
    QObject::connect(streamController, &StreamController::signalUpdateRate,
                    playerController, &PlayerController::updateRate);
    QObject::connect(streamController, &StreamController::signalUpdatePlaybackState,
                    playerController, &PlayerController::updatePlaybackState);
    QObject::connect(streamController, &StreamController::sendMessageToStatusBar,
                    &player, &Player::showMessageInStatusBar);

    return a.exec();
}
