#include "streamcontroller.h"
#include "player.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StreamController playerController;
    Player player;
    player.show();

    QObject::connect(&player, &Player::RequestToObtainSource,
                    &playerController, &StreamController::RequestedToObtainSource);
    QObject::connect(&player, &Player::RequestToStream,
                    &playerController, &StreamController::RequestedToStream);
    QObject::connect(&player, &Player::RequestToPauseStream,
                    &playerController, &StreamController::RequestedToPauseStream);

    QObject::connect(&playerController, &StreamController::SignalSourceObtained,
                    &player, &Player::HandleSourceObtained);



    return a.exec();
}
