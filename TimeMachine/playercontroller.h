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

    //libvlc_instance_t *mVlcInstance;
    //libvlc_media_player_t *mMediaPlayer;
    //libvlc_media_t *mMedia;

    bool isPlaying = false;
    bool isIntendedToPlay = false;
    float playSpeed = 1;
    std::string inputLocation;
    qint32 currentPlayTime;
    float currentRate;
    QString currentFilename;
    qint32 currentFileEndTime;
    qint32 nextFileStartTime;

    QTimer *playTimer;

    PlaybackState playbackState;

signals:
    void requestToObtainSource(quint32 requestTime, float playSpeed);
    void requestToStreamRealTime();
    void requestToStreamArchive();
    void requestToPauseRealTime();
    void requestToPauseArchive();
    void requestToStop();
    void requestToSpeedUp();
    void requestToSpeedDown();

    void updateTimeBarSlider(qint32 playTime);

public slots:
    void handleSourceObtained();
    void attemptToPlayStream();

    void setPlayPosition(qint32 requestTime);
    void startPlayTimer(qint32 startTime);
    void stopTimeSlider();
    void updateRate(float rate);
    void playTimerShot();

    void playButtonClicked();
    void playRealTimeButtonClicked();
    void testInputButtonClicked();
    void speedUpButtonClicked();
    void speedDownButtonClicked();
    void stepForwardButtonClicked();
    void stepBackButtonClicked();
    void stopButtonClicked();

    void updatePlaybackState(PlaybackState newState);

};
