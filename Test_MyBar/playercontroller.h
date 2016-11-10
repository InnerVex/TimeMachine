#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QDebug>

#include "include/vlc/vlc.h"
#include <unistd.h>

#include <playerdefinitions.h>

class PlayerController : public QObject
{
    Q_OBJECT
public:
    explicit PlayerController(QObject *parent = 0);

private:
    //Переменные libVLC
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;

    SourceData sourceData;
    long sourceMargin;

signals:
    //Сигнал проигрывателю о том, что запущен новый источник
    void SignalSourceObtained();

public slots:
    //Слот по сигналам от проигрывателя
    void RequestedToObtainSource(long requestTime, float playSpeed);
    void RequestedToStream(float playSpeed);
    void RequestedToPauseStream();

};

#endif // PLAYERCONTROLLER_H
