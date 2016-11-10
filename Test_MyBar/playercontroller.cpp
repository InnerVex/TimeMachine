#include "playercontroller.h"

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{
    //Формирование строки выхода
    string dstParam = "rtp{sdp=rtsp://localhost:5544/}";
    string soutLine = "--sout=#transcode{vcodec=MJPG,vb=0,scale=1,acodec=mpga,ab=128,channels=2,samplerate=44100}:";
    soutLine.append(dstParam);

    //Объекты libVLC для стрима
    const char * const vlc_args[] = {
        "--verbose=0",
        soutLine.c_str()};
    mVlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    mMediaPlayer = libvlc_media_player_new (mVlcInstance);
}

void PlayerController::RequestedToObtainSource(long requestTime, float playSpeed)
{
    //TODO::Запросить из БД файл и время его начала, рассчитать временное смещение
    sourceData = SourceData("D:\\example.avi", 0, 100);
    sourceMargin = 100;

    //Отправить сигнал о получении данных об источнике
    emit SignalSourceObtained();
}

void PlayerController::RequestedToStream(float playSpeed)
{
    //Начать воспроизводить источник с заданными параметрами
    mMedia = libvlc_media_new_path(mVlcInstance, "D:\\example.avi");
    //mMedia = libvlc_media_new_path(mVlcInstance, "D:\\filename.mp4");
    libvlc_media_player_set_media (mMediaPlayer, mMedia);
    //libvlc_media_player_set_position(mMediaPlayer, (float)sourceMargin);


    libvlc_media_player_play(mMediaPlayer);

    libvlc_media_player_set_position(mMediaPlayer, 0.5f);
    //libvlc_media_player_set_rate(mMediaPlayer, 1.5f);
}

void PlayerController::RequestedToPauseStream()
{
    //TODO::Остановить стрим

}
