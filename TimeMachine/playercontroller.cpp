#include "playercontroller.h"

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

    connect(player->timeBar, &TimeBar::sendMessageToStatusBar,
            player, &Player::showMessageInStatusBar);
}


void PlayerController::handleSourceObtained()
{
    //Возобновить воспроизведение, если воспроизводилось до этого
    if(isIntendedToPlay && !isPlaying)
    {
        emit requestToStream(playSpeed);

        //TODO - Как-то понять, что стрим работает и продолжить воспроизведение
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //playStream();
    }
}

void PlayerController::playStream()
{
    //Объект медиаданных из источника
    mMedia = libvlc_media_new_location(mVlcInstance, inputLocation.c_str());
    libvlc_media_player_set_media (mMediaPlayer, mMedia);

    //Дескриптор окна
    int windid = player->ui->videoFrame->winId();
    libvlc_media_player_set_hwnd(mMediaPlayer, (void*)windid );

    //Старт проигрывания
    libvlc_media_player_play(mMediaPlayer);

    isPlaying=true;
    isIntendedToPlay = true;
}

void PlayerController::setPlayPosition(quint32 requestTime)
{
    //TODO::Отправка запроса на обновление стрима
}

void PlayerController::playTimerShot()
{
    //TODO: Синхронизация с интерфейсом

}

void PlayerController::playButtonClicked()
{
    playStream();
}

//TOTALLY TEST CODE
void PlayerController::testInputButtonClicked()
{
    isIntendedToPlay = true;
    quint32 requestedTime = player->ui->testTimeInput->dateTime().toTime_t();
    player->showMessageInStatusBar("RequestedTime: " + QString::number(requestedTime));

    if (requestedTime == 0)
    {
        player->showMessageInStatusBar("RequestedTime Invalid");
    }
    else
    {
        emit requestToObtainSource(requestedTime, 1);
    }

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
