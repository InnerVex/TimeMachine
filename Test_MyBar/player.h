#pragma once

#include <QMainWindow>
#include <QDebug>

#include "vlc/vlc.h"

#include <timebar.h>
#include <playerdefinitions.h>

namespace Ui {
class Player;
}

class Player : public QMainWindow
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();
    Ui::Player *ui;


};
