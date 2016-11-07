#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <playerdefinitions.h>
#include <player.h>

class PlayerController : public QObject
{
    Q_OBJECT
public:
    explicit PlayerController(Player* _player, QObject *parent = 0);

private:
    Player* player;

signals:
    //Сигнал проигрывателю о том, что запущен новый источник
    void SignalSourceObtained();

public slots:
    //Слот по сигналу от проигрывателя с запросом изменения параметров источника
    void ReceiveRequestForSource(long requestTime, float playSpeed);


};

#endif // PLAYERCONTROLLER_H
