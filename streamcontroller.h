#pragma once

#include <QObject>

#include "vlc/vlc.h"
#include <unistd.h>

#include <playerdefinitions.h>

class StreamController : public QObject
{
    Q_OBJECT
public:
    explicit StreamController(QObject *parent = 0);

private:
    //Переменные libVLC
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;

    SourceData sourceData;
    long sourceMargin;

signals:
    //Сигнал проигрывателю о том, что запущен новый источник
    void signalSourceObtained();

public slots:
    //Слот по сигналам от проигрывателя
    void requestedToObtainSource(quint32 requestTime, float playSpeed);
    void requestedToStream(float playSpeed);
    void requestedToPauseStream();

};
