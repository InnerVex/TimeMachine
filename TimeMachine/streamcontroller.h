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
    explicit StreamController(Player *_player, QObject *parent = 0);
    void replenishVideoPatchesBuffer();
    void openFileForBuffering(QString filename, bool withOffset = false);
    void loadVideoPatchInMemory(qint32 bytesToBuffer);
    void createImemInstance();
    void createRTPSInstance();

    QString currentFilename;
    QFile *currentQFile;
    qint32 currentFileBytes;
    qint32 currentFileBytePosition;
    float percentOffset;
    qint32 totalBytesBuffered;

private:
    libvlc_instance_t *vlcInstanceImem;
    libvlc_instance_t *vlcInstanceRtsp;
    libvlc_media_player_t *mediaPlayerImem;
    libvlc_media_player_t *mediaPlayerRtsp;
    libvlc_media_t *mediaImem;
    libvlc_media_t *mediaRtsp;
    ImemData *mImemData;

    QTimer *mAttemptTimer;
    QTimer *mReplenishTimer;
    qint32 requestTime;
    bool attemptingToStartStream;
    bool streamingImem;
    bool streamingRtsp;
    bool signalledStartTimer;
    bool imemInstanceReady;
    bool rtspInstanceReady;
    float currentRate;
    Player *player;
    int windid;

    void startWaitingForStreamStart(bool isImem);

signals:
    void signalSourceObtained();
    void signalStreamStarted();
    void signalTimerStart(qint32 startTime);
    void signalUpdateRate(float rate);
    void sendMessageToStatusBar(QString message);

public slots:
    void requestedToObtainSource(quint32 requestTime, float playSpeed);
    void requestedToStreamArchive();
    void requestedToStreamRealTime();
    void requestedToPauseStream();
    void streamSpeedUp();
    void streamSpeedDown();
};
