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
        /*
        {

            Slicer slicer;
            const char* input ="rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
            int numberOfSlices = 3;
            slicer.makeMultipleSlices(input, numberOfSlices);

        }

        {
        QDateTime SelectDateTime(QDate(2000,01,01),QTime(00,00,30));
        qint32 time = SelectDateTime.toTime_t();
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
        Player player;
        PlayerController *playerController = new PlayerController(&player);
        StreamController *streamController = new StreamController();
        streamController->player = &player; //TMP::Для прямого воспроизведения Дешинковщиком
        player.show();

        //Сигналы от Проигрывателя и GUI к Дешинковщику
        QObject::connect(playerController, &PlayerController::requestToObtainSource,
                        streamController, &StreamController::requestedToObtainSource);
        QObject::connect(playerController, &PlayerController::requestToStream,
                        streamController, &StreamController::requestedToStream);
        QObject::connect(playerController, &PlayerController::requestToRealTimeStream,
                        streamController, &StreamController::requestedToStreamRealTime);
        QObject::connect(playerController, &PlayerController::requestToPauseStream,
                        streamController, &StreamController::requestedToPauseStream);
        QObject::connect(player.ui->buttonSpeedUp, &QPushButton::clicked,
                        streamController, &StreamController::streamSpeedUp);
        QObject::connect(player.ui->buttonSpeedDown, &QPushButton::clicked,
                        streamController, &StreamController::streamSpeedDown);

        //Сигналы от Дешинковщика к Проигрывателю и GUI
        QObject::connect(streamController, &StreamController::signalSourceObtained,
                        playerController, &PlayerController::handleSourceObtained);
        QObject::connect(streamController, &StreamController::signalStreamStarted,
                        playerController, &PlayerController::attemptToPlayStream);
        QObject::connect(streamController, &StreamController::signalTimerStart,
                        playerController, &PlayerController::startPlayTimer);
        QObject::connect(streamController, &StreamController::signalUpdateRate,
                        playerController, &PlayerController::updateRate);
        QObject::connect(streamController, &StreamController::sendMessageToStatusBar,
                        &player, &Player::showMessageInStatusBar);

        return a.exec();
    }
