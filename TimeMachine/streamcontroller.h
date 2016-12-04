#pragma once

#include <QObject>
#include <QTimer>
#include <QFile>

#include "vlc/vlc.h"
#include <unistd.h>
#include <assert.h>
#include <player.h>
#include <ui_player.h>
#include <player.h>
#include <playerdefinitions.h>


#define VIDEO_PATCH_BYTES           128000
#define VIDEO_PATCHES_TOTAL_BYTES   640000
#define START_SIGNAL_DELAY          500

#define RATE_MULTIPLY   2
#define RATE_MAX        4
#define RATE_MIN        0.25

class StreamController;

class ImemData
{
public:
    ImemData() {}
    ~ImemData() {}
    std::vector<VideoPatchData> videoPatches;
    char* cookieString;
    StreamController *controller;
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
    void openFileForBuffering(QString filename, bool withOffset = false);
    void loadVideoPatchInMemory(qint32 bytesToBuffer);
    void createImemInstance();

    QString currentFilename;
    QFile *currentQFile;
    qint32 currentFileBytes;
    qint32 currentFileBytePosition;
    float percentOffset;
    qint32 totalBytesBuffered;

    float currentRate;

    Player *player;

private:
    //Переменные libVLC
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;
    ImemData *mImemData;

    QTimer *mAttemptTimer;
    QTimer *mReplenishTimer;
    qint32 requestTime;
    bool attemptingToStartStream;
    bool streaming;
    bool signalledStartTimer;
    bool imemStreamReady;

    void startWaitingForStreamStart();

signals:
    //Сигнал проигрывателю о том, что запущен новый источник
    void signalSourceObtained();
    void signalStreamStarted();
    void signalTimerStart(qint32 startTime);
    void signalUpdateRate(float rate);
    void sendMessageToStatusBar(QString message);

public slots:
    //Слот по сигналам от проигрывателя
    void requestedToObtainSource(quint32 requestTime, float playSpeed);
    void requestedToStream();
    void requestedToStreamRealTime();
    void requestedToPauseStream();
    void streamSpeedUp();
    void streamSpeedDown();
};
