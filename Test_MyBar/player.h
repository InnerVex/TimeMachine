#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include "include/vlc/vlc.h"
#include <timebar.h>
#include <playerdefinitions.h>

using namespace std;

namespace Ui {
class Player;
}

class Player : public QMainWindow
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();

private:
    Ui::Player *ui;

    SourceList sourceList;
    long sourceMargin;

    //Параметры воспроизведения
    bool isPlaying = false;
    bool isIntendedToPlay = false;
    float playSpeed = 1;

    //Переменные libVLC
    libvlc_instance_t *mVlcInstance;
    libvlc_media_player_t *mMediaPlayer;
    libvlc_media_t *mMedia;

    //Таймер, сопровождающий воспроизведение
    QTimer *mPlayTimer;

signals:
    //Запросить изменение параметров источника
    void RequestSource(long requestTime, float playSpeed);

public slots:
    //Слот по сигналу от контроллера к продолжению воспроизведения
    void HandleSourceObtained();

    //Начать воспроизведение из источника
    void PlaySource(SourceData sourceData);

    //Запрос пользователя на новый момент воспроизведения
    void SetPlayPosition(long requestTime);

    //Слот таймера синхронизации интерфейса с воспроизведением
    void PlayTimerShot();
};

#endif // PLAYER_H
