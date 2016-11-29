#include "playercontroller.h"
#include "iostream"
#include <chrono>
#include <thread>

#include <ui_player.h>



PlayerController::PlayerController(Player *_player, QObject *parent) :
    QObject(parent),
    player(_player)
{
    //Объекты libVLC для воспроизведения входящего стрима
    const char * const vlc_args[] = {"--verbose=0"};
    mVlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    mMediaPlayer = libvlc_media_player_new (mVlcInstance);

    inputLocation = "rtsp://localhost:5544/";

    //Коннекты элементов управления
    connect(player->ui->buttonPlay, &QPushButton::clicked,
            this, &PlayerController::playButtonClicked);

    connect(player->ui->testTimeButton, &QPushButton::clicked,
            this, &PlayerController::testInputButtonClicked);

    connect(player->ui->buttonStepBack, &QPushButton::clicked,
            this, &PlayerController::stepBackButtonClicked);
    
    connect(player->ui->buttonStepForward, &QPushButton::clicked,
            this, &PlayerController::stepForwardButtonClicked);
    
    connect(player->ui->buttonSpeedUp, &QPushButton::clicked,
            this, &PlayerController::speedUpButtonClicked);
    
    connect(player->ui->buttonSpeedDown, &QPushButton::clicked,
            this, &PlayerController::speedDownButtonClicked);
    
    connect(player->ui->buttonRealTime, &QPushButton::clicked,
            this, &PlayerController::playRealTimeButtonClicked);

    connect(player->timeBar, &TimeBar::sendMessageToStatusBar,
            player, &Player::showMessageInStatusBar);
    
}


void PlayerController::handleSourceObtained()
{
    //Воспроизведение продолжается, если воспроизводилось до этого
    //if(isIntendedToPlay && !isPlaying)
    {
        //Запрос стримеру на начало стрима и старт попыток подхватить стрим
        emit requestToStream(playSpeed);
        startAttemptsToPlayStream();
    }
}

void PlayerController::startAttemptsToPlayStream()
{
    mAttemptTimer = new QTimer();
    connect(mAttemptTimer, &QTimer::timeout, [=]()
    {
        if(libvlc_media_player_is_playing(mMediaPlayer) != 0)
        {
            //attemptToPlayStream();
            player->showMessageInStatusBar("Stream started");
            mAttemptTimer->stop();
            isPlaying=true;
            isIntendedToPlay = true;
        }
        else
        {
            attemptToPlayStream();
        }
    });

    mAttemptTimer->start(3000);
}

void PlayerController::attemptToPlayStream()
{
    //Объект медиаданных из источника
    mMedia = libvlc_media_new_location(mVlcInstance, inputLocation.c_str());
    libvlc_media_player_set_media (mMediaPlayer, mMedia);

    //Дескриптор окна
    int windid = player->ui->videoFrame->winId();
    libvlc_media_player_set_hwnd(mMediaPlayer, (void*)windid );

    //Старт проигрывания
    libvlc_media_player_play(mMediaPlayer);
}

void PlayerController::setPlayPosition(quint32 requestTime)
{
    //TODO::Отправка запроса на обновление стрима

}

void PlayerController::playTimerShot()
{
    //TODO::Синхронизация с интерфейсом

}

void PlayerController::playButtonClicked()
{
    //TODO::Различные действия в зависимости от того, воспроизводится ли стрим
    //Запрос стримеру на начало стрима и старт попыток подхватить стрим
    libvlc_media_player_stop(mMediaPlayer);
    emit requestToStream(playSpeed);
    startAttemptsToPlayStream();
}

//TOTALLY TEST CODE
void PlayerController::testInputButtonClicked()
{
    quint32 requestedTime = player->ui->testTimeInput->dateTime().toTime_t();
    player->showMessageInStatusBar("RequestedTime: " + QString::number(requestedTime));

    emit requestToObtainSource(requestedTime, 1);
}
//Real Time Stream
void PlayerController::playRealTimeButtonClicked()
{
    libvlc_media_player_stop(mMediaPlayer);
    emit requestToRealTimeStream();
    startAttemptsToPlayStream();
}

void PlayerController::startToPlayRealTimeStream()
{

}

/*
void TestPlayer::updateInterface()
{
    if(!mIsPlaying)
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (mMediaPlayer);
    if (curMedia == NULL)
        return;

    float pos=libvlc_media_player_get_position (mMediaPlayer);
    int sliderPos=(int)(pos*(float)(POSITION_RESOLUTION));
    mPositionSlider->setValue(sliderPos);
    int volume=libvlc_audio_get_volume (mMediaPlayer);
    mVolumeSlider->setValue(volume);
}
*/
