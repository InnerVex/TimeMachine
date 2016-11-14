#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    setWindowTitle(tr("Player"));

    //Создание элементов управления
    TimeBar *timeBar = new TimeBar(ui->centralwidget);
    timeBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->centralwidget->layout()->addWidget(timeBar);
}

Player::~Player()
{
    delete ui;
}

