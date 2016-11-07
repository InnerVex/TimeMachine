#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);

    TimeBar *timeBar = new TimeBar(ui->centralwidget);
    timeBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->centralwidget->layout()->addWidget(timeBar);


    const char * const vlc_args[] = {"--verbose=2"};
    mVlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);  //tricky calculation of the char space used
    mMediaPlayer = libvlc_media_player_new (mVlcInstance);


    //TEST
    SourceData data("D:\\example.avi", 0, 100);
    PlaySource(data);

    setWindowTitle(tr("Player"));
}

Player::~Player()
{
    delete ui;
}

//Слот: Контроллер присылает список новых источников вместо или в дополнение к текущему
void Player::HandleSourceObtained()
{
    if(isIntendedToPlay && !isPlaying)
    {

    }
}

//Начать воспроизведение источника с заданным смещением
void Player::PlaySource(SourceData sourceData)
{
    //Объект медиаданных из источника
    mMedia = libvlc_media_new_path(mVlcInstance, sourceData.path.c_str());
    libvlc_media_player_set_media(mMediaPlayer, mMedia);
    //libvlc_media_player_set_position(mMediaPlayer, sourceMargin);

    //Дескриптор окна
    int windid = ui->videoFrame->winId();
    libvlc_media_player_set_hwnd(mMediaPlayer, (void*)windid );

    //Старт проигрывания
    libvlc_media_player_play(mMediaPlayer);
    isPlaying=true;
    isIntendedToPlay = true;
}

//Слот: Пользователь запрашивает смену момента воспроизведения
void Player::SetPlayPosition(long requestTime)
{
    if(!sourceList.empty())
    {
        //Если момент в пределах текущего источника
        if(requestTime < sourceList[0].sourceEndTime && requestTime > sourceList[0].sourceStartTime)
        {
            sourceMargin = sourceList[0].sourceStartTime + requestTime;
            libvlc_media_player_set_position(mMediaPlayer, sourceMargin);
            return;
        }
    }

    if(isPlaying)
    {
       libvlc_media_player_stop(mMediaPlayer);
       isPlaying = false;
    }

    //Запрос на новый список источников и смещение
    emit RequestSource(requestTime, playSpeed);
}

void Player::PlayTimerShot()
{
    //TODO: Проверка на окончание текущего источника и запрос на дополнение списка источников

    //TODO: Синхронизация с интерфейсом

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
