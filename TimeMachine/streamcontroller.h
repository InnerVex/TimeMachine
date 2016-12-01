#pragma once

#include <QObject>

#include "vlc/vlc.h"
#include <unistd.h>
#include <assert.h>
#include <player.h>
#include <ui_player.h>
#include <playerdefinitions.h>


#define VIDEO_PATCH_BYTES 10485760
#define VIDEO_PATCHES_TOTAL_BYTES 52428800

class ImemData
{
public:
    ImemData() {}
    ~ImemData() {}
    std::vector<VideoPatchData> videoPatches;
    char* cookieString;
};

template<typename T>
void pop_front(std::vector<T>& vec)
{
    assert(!vec.empty());
    vec.erase(vec.begin());
}

class StreamController : public QObject
{
    Q_OBJECT
public:
    explicit StreamController(QObject *parent = 0);
    void replenishVideoPatchesBuffer();
    void openFileForBuffering(const char* filename, bool withOffset = false);
    void loadVideoPatchInMemory(qint32 bytesToBuffer);

private:
    Player *player;
    //Переменные libVLC
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;
    ImemData *mImemData;

    const char* currentFilename;
    FILE* currentFile;
    qint32 currentFileBytes;
    qint32 currentFileBytePosition;
    float percentOffset;
    qint32 totalBytesBuffered;

signals:
    //Сигнал проигрывателю о том, что запущен новый источник
    void signalSourceObtained();

public slots:
    //Слот по сигналам от проигрывателя
    void requestedToObtainSource(quint32 requestTime, float playSpeed);
    void requestedToStream();
    void requestedToPauseStream();

};
