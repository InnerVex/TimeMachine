#pragma once

#include <QObject>
#include <QTimer>

#include "vlc/vlc.h"

#include <timebar.h>
#include <playerdefinitions.h>
#include <player.h>

class PlayerController : public QObject
{
    Q_OBJECT
public:
    PlayerController(Player *_player, QObject *parent = 0);

private:
    Player *player;

    long sourceMargin;
    //
    //Параметры воспроизведения
    bool isPlaying = false;
    bool isIntendedToPlay = false;
    float playSpeed = 1;
    std::string inputLocation;
    qint32 currentPlayTime;

    //Переменные libVLC
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;

    //Таймер, сопровождающий воспроизведение
    QTimer *mAttemptTimer;
    QTimer *mPlayTimer;

signals:
    //Запросить изменение параметров источника
    void requestToObtainSource(quint32 requestTime, float playSpeed);
    void requestToRealTimeStream();
    void requestToStream();
    void requestToPauseStream();
    void requestStepForward(quint32 step);
    void requestStepBack(quint32 step);
    void requestSpeedUp(float currentPlaySpeed);
    void requestSpeedDown(float currentPlaySpeed);

    void updateTimeBarScroller(qint32 playTime);

public slots:
    //Слот по сигналу от контроллера к продолжению воспроизведения
    void handleSourceObtained();

    //Начать воспроизведение из источника
    void attemptToPlayStream();
    void startToPlayRealTimeStream();

    //Функции, связанные с таймбаром
    void setPlayPosition(qint32 requestTime);

    //Слот таймера синхронизации интерфейса с воспроизведением
    void startPlayTimer(qint32 startTime);
    void playTimerShot();

    //Слоты кнопок управления воспроизведением
    void playButtonClicked();
    void playRealTimeButtonClicked();
    void testInputButtonClicked();
    void stepForwardButtonClicked();
    void stepBackButtonClicked();
    void speedUpButtonClicked();
    void speedDownButtonClicked();
};
