#include "streamcontroller.h"
#include "playercontroller.h"
#include "player.h"
#include "insert.h"
#include "select.h"
#include <iostream>
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QDateTime firstDateTime(QDate(2000,1,1),QTime(00,00,00));
    quint32 time = firstDateTime.toTime_t();
    QString iFileName = QFileInfo("slice_0.ts").fileName();
    //QDir iFilePath = QFileInfo("slice_0.ts").dir();
    QString iSourceName = "CAM01";
    QString iSourceAdress = "1.1.1.1";
    qint32  iOffset = 0;
    for(int i = 0; i < 133; ++i)
    {
        iOffset++;
        Insert::insert(++time,iFileName,iSourceName,iSourceAdress,"home/",iOffset);
    }

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

    QApplication a(argc, argv);

    Player player;
    PlayerController *playerController = new PlayerController(&player);
    StreamController streamController;
    player.show();

    QObject::connect(playerController, &PlayerController::requestToObtainSource,
                    &streamController, &StreamController::requestedToObtainSource);
    QObject::connect(playerController, &PlayerController::requestToStream,
                    &streamController, &StreamController::requestedToStream);
    QObject::connect(playerController, &PlayerController::requestToPauseStream,
                    &streamController, &StreamController::requestedToPauseStream);
    QObject::connect(&streamController, &StreamController::signalSourceObtained,
                    playerController, &PlayerController::handleSourceObtained);



    return a.exec();
}
