#include "playercontroller.h"

PlayerController::PlayerController(Player* _player, QObject *parent) : QObject(parent)
{
    player = _player;

    //Принятие запроса на источники и отправка источников
    connect(player, SIGNAL(RequestSources(long,float,long)),
                           this, SLOT(ReceiveRequestForSources(long,float,long)));
    connect(this, SIGNAL(SendSources(QList<SourceData>,long,bool)),
                         player, SLOT(ReceiveSources(QList<SourceData>,long,bool)));
}

void PlayerController::ReceiveRequestForSource(long requestTime, float playSpeed)
{
    //TODO::Запросить из БД файл и время его начала по запросу

    //Отправить сигнал к продолжению воспроизведения
    emit SignalSourceObtained();

    //TODO::Начать воспроизводить в поток новый источник с заданной скоростью
}
