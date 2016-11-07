#include "player.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Player player;
    player.show();

    return a.exec();
}
