#include "streamcontroller.h"

#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "select.h"

#define TESTFILE_DURATION 133.0

StreamController::StreamController(QObject *parent) : QObject(parent)
{
    //Формирование строки выхода
    //std::string dstParam = "rtp{sdp=rtsp://localhost:5544/}";
    //string soutLine = "--sout=#transcode{vcodec=h264,vb=0,scale=1,acodec=mpga,ab=128,channels=2,samplerate=44100}:";
    //std::string soutLine = "--sout=";
    //soutLine.append(dstParam);

    std::string soutLine = "--sout=#rtp{dst=localhost,port=5544,sdp=rtsp://localhost:5544/}";
    //Объекты libVLC для стрима
    const char * const vlc_args[] = {
        "--verbose=2",
        soutLine.c_str()};
    mVlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    mMediaPlayer = libvlc_media_player_new (mVlcInstance);
}

void StreamController::requestedToObtainSource(quint32 requestTime, float playSpeed)
{
    libvlc_media_player_stop(mMediaPlayer);

    //Получение данных из БД
    sourceFileName = Select::selectFile(requestTime).toStdString().c_str();
    //sourceMargin = Select::selectDateTime(requestTime);

    //Запускаем проигрывание
    mMedia = libvlc_media_new_path(mVlcInstance, sourceFileName);
    libvlc_media_player_set_media (mMediaPlayer, mMedia);

    //Получаем продолжительность
    libvlc_media_parse(mMedia);
    float duration = libvlc_media_get_duration(mMedia);
    duration *= 0.001;

    libvlc_media_player_play(mMediaPlayer);
    float position = sourceMargin / duration;
    libvlc_media_player_set_position(mMediaPlayer, position);

    libvlc_media_player_pause(mMediaPlayer);

    //Отправить сигнал о получении данных об источнике
    emit signalSourceObtained();
}

void StreamController::requestedToStream(float playSpeed)
{
    QString source = "rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
    sourceFileName = source.toStdString().c_str();
    mMedia = libvlc_media_new_location(mVlcInstance, sourceFileName);
    libvlc_media_player_set_media (mMediaPlayer, mMedia);
    libvlc_media_player_play(mMediaPlayer);


}

void StreamController::requestedToPauseStream()
{
    //TODO::Остановить стрим

}
