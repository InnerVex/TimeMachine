#include "streamcontroller.h"
#include "playercontroller.h"
#include "player.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
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
