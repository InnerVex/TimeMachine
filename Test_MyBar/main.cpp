#include "playercontroller.h"
#include "player.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PlayerController playerController;
    Player player;
    player.show();

    QObject::connect(&player, &Player::RequestToObtainSource,
                    &playerController, &PlayerController::RequestedToObtainSource);
    QObject::connect(&player, &Player::RequestToStream,
                    &playerController, &PlayerController::RequestedToStream);
    QObject::connect(&player, &Player::RequestToPauseStream,
                    &playerController, &PlayerController::RequestedToPauseStream);

    QObject::connect(&playerController, &PlayerController::SignalSourceObtained,
                    &player, &Player::HandleSourceObtained);



    return a.exec();
}
