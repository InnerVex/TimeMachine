#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);

    //Устанавливаем фон окна воспроизведения
    QPalette Pal(ui->videoFrame->palette());
    Pal.setColor(QPalette::Background, Qt::black);
    ui->videoFrame->setAutoFillBackground(true);
    ui->videoFrame->setPalette(Pal);

    //Создание элементов управления
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    timeBar = new TimeBar(ui->dateTimeEdit, ui->centralwidget);
    timeBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->timeBarContainerWidget->layout()->addWidget(timeBar);
}

Player::~Player()
{
    delete ui;
}

void Player::showMessageInStatusBar(QString message)
{
    ui->statusbar->showMessage(message);
}
