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

    //Объекты libVLC для воспроизведения входящего стрима
    const char * const vlc_args[] = {"--verbose=0"};
    mVlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    mMediaPlayer = libvlc_media_player_new (mVlcInstance);

    inputLocation = "rtsp://localhost:5544/";

    //Коннекты элементов управления
    connect(ui->playButton, &QPushButton::clicked,
            this, &Player::PlayButtonClicked);
}

Player::~Player()
{
    delete ui;
}

void Player::HandleSourceObtained()
{
    //Возобновить воспроизведение, если воспроизводилось до этого
    if(isIntendedToPlay && !isPlaying)
    {
        emit RequestToStream(playSpeed);

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        PlayStream();
    }
}

void Player::PlayStream()
{
    //Объект медиаданных из источника
    mMedia = libvlc_media_new_location(mVlcInstance, inputLocation.c_str());
    libvlc_media_player_set_media (mMediaPlayer, mMedia);

    //Дескриптор окна
    int windid = ui->videoFrame->winId();
    libvlc_media_player_set_hwnd(mMediaPlayer, (void*)windid );

    //Старт проигрывания
    libvlc_media_player_play(mMediaPlayer);

    isPlaying=true;
    isIntendedToPlay = true;
}

void Player::SetPlayPosition(long requestTime)
{   
    //TODO::Отправка запроса на обновление стрима
}

void Player::PlayTimerShot()
{
    //TODO: Синхронизация с интерфейсом

}

void Player::PlayButtonClicked()
{
    //isIntendedToPlay = !isIntendedToPlay;
    isIntendedToPlay = true;

    //TOTALLY TEST CODE
    emit RequestToObtainSource(5, 1);
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
