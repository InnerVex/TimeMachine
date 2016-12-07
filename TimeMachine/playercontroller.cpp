#include "playercontroller.h"
#include "iostream"
#include <chrono>
#include <thread>
#include <QtGlobal>
#include <select.h>
#include <ui_player.h>

PlayerController::PlayerController(Player *_player, QObject *parent) :
    QObject(parent),
    player(_player)
{
    //Объекты libVLC для воспроизведения входящего стрима
    /*const char * const vlc_args[] = {"--verbose=0"};
    mVlcInstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    mMediaPlayer = libvlc_media_player_new (mVlcInstance);
    inputLocation = "rtsp://localhost:5544/";*/

    currentRate = 1;
    playbackState = PlaybackState::stopped;

    //Коннекты элементов управления
    connect(player->ui->buttonPlay, &QPushButton::clicked,
            this, &PlayerController::playButtonClicked);
    connect(player->ui->testTimeButton, &QPushButton::clicked,
            this, &PlayerController::testInputButtonClicked);
    connect(player->ui->buttonStop, &QPushButton::clicked,
            this, &PlayerController::stopButtonClicked);
    connect(player->ui->buttonSpeedUp, &QPushButton::clicked,
            this, &PlayerController::speedUpButtonClicked);
    connect(player->ui->buttonSpeedDown, &QPushButton::clicked,
            this, &PlayerController::speedDownButtonClicked);
    connect(player->ui->buttonStepBack, &QPushButton::clicked,
            this, &PlayerController::stepBackButtonClicked);
    connect(player->ui->buttonStepForward, &QPushButton::clicked,
            this, &PlayerController::stepForwardButtonClicked);
    connect(player->ui->buttonRealTime, &QPushButton::clicked,
            this, &PlayerController::playRealTimeButtonClicked);
    connect(player->ui->buttonShowSlider, &QPushButton::clicked,
            this, &PlayerController::showSliderButtonClicked);

    connect(player->timeBar, &TimeBar::sendMessageToStatusBar,
            player, &Player::showMessageInStatusBar);
    connect(player->timeBar, &TimeBar::setPlayTime,
            this, &PlayerController::setPlayPosition);
    connect(player->ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged,
            player->timeBar, &TimeBar::setFVT);
    connect(this, &PlayerController::updateTimeBarSlider,
            player->timeBar, &TimeBar::setSliderPosition);


    playTimer = new QTimer();
    connect(playTimer, &QTimer::timeout,
            this, &PlayerController::playTimerShot);
}

/**
 * @brief PlayerController::handleSourceObtained
 * Запрашивает воспроизведение архивов, получив сигнал об окончании пребуферизации.
 */
void PlayerController::handleSourceObtained()
{
    emit requestToStreamArchive();
}

/**
 * @brief PlayerController::attemptToPlayStream
 * Deprecated
 */
void PlayerController::attemptToPlayStream()
{
    /*
    //Объект медиаданных из источника
    mMedia = libvlc_media_new_location(mVlcInstance, inputLocation.c_str());
    libvlc_media_player_set_media (mMediaPlayer, mMedia);

    //Дескриптор окна
    int windid = player->ui->videoFrame->winId();
    libvlc_media_player_set_hwnd(mMediaPlayer, (void*)windid );

    //Старт проигрывания
    libvlc_media_player_play(mMediaPlayer);
    */
}

/**
 * @brief PlayerController::setPlayPosition
 * @param requestTime - момент архива, с которого нужно начать воспроизведение.
 * Запрашивает у Дешинковщика начало воспроизведения архива.
 */
void PlayerController::setPlayPosition(qint32 requestTime)
{
    emit requestToObtainSource(requestTime, 1);
}

/**
 * @brief PlayerController::startPlayTimer
 * @param startTime - момент времени, с которого начал воспроизведение Дешинковщик.
 * Запускает таймер для синхронизации интерфейса и
 * определяет момент перехода к следующему архивному файлу.
 */
void PlayerController::startPlayTimer(qint32 startTime)
{
    if(startTime != -1)
    {
        currentPlayTime = startTime;
    }
    setTimeLabelText(currentPlayTime);
    playTimer->start(1000 / currentRate);

    currentFilename = Select::selectFile(currentPlayTime);
    currentFileEndTime = Select::selectDateTime(currentFilename) + Select::selectDuration(currentPlayTime) * 0.001;
    nextFileStartTime = Select::selectNextDateTime(currentPlayTime);
}

/**
 * @brief PlayerController::stopPlayTimer
 * Останавливает таймер синхронизации интерфейса и скрывает кирпич
 */
void PlayerController::stopTimeSlider()
{
    playTimer->stop();
    emit updateTimeBarSlider(-1);
}

/**
 * @brief PlayerController::updateRate
 * @param rate - новая скорость воспроизведения.
 * Изменяет скорость воспроизведения.
 */
void PlayerController::updateRate(float rate)
{
    currentRate = rate;
    playTimer->setInterval(1000 / currentRate);
}

/**
 * @brief PlayerController::setTimeLabelText
 * @param text - текст для отображения рядом с таймбаром
 */
void PlayerController::setTimeLabelText(QString text)
{
    player->ui->playTimeLabel->setText(text);
}

/**
 * @brief PlayerController::setTimeLabelText
 * @param time - проигрываемое время для отображения рядом с таймбаром
 */
void PlayerController::setTimeLabelText(qint32 time)
{
    player->ui->playTimeLabel->setText(QDateTime::fromTime_t(time).toString("dd'.'MM'.'yyyy' 'hh':'mm':'ss"));
}

/**
 * @brief PlayerController::playTimerShot
 * Инкремент момента проигрывания и проверка на переход между файлами архива/реалтаймом.
 */
void PlayerController::playTimerShot()
{
    currentPlayTime++;
    if(currentPlayTime >= currentFileEndTime)
    {
        if(nextFileStartTime == -1)
        {
            //Архив закончился
            playTimer->stop();
            emit requestToStreamRealTime();
        }
        else
        {
            currentPlayTime = nextFileStartTime;
            currentFilename = Select::selectFile(currentPlayTime);
            currentFileEndTime = Select::selectDateTime(currentFilename) + Select::selectDuration(currentPlayTime) * 0.001;
            nextFileStartTime = Select::selectNextDateTime(currentPlayTime);
        }
    }

    setTimeLabelText(currentPlayTime);
    emit updateTimeBarSlider(currentPlayTime);
}

/**
 * @brief PlayerController::playButtonClicked
 * Обрабатывает нажатие на кнопку воспроизведения. В случае остановки всех инстансов запрашивается
 * воспроизведение архива с первого видимого на таймбаре момента. В случае воспроизведения запрашивается
 * пауза работающего инстанса. Изменения в интерфейсе происходят по приходу ответного сигнала.
 */
void PlayerController::playButtonClicked()
{
    switch (playbackState) {
    case playingImem:
        emit requestToPauseArchive();
        playTimer->stop();
        break;
    case playingRtsp:
        emit requestToPauseRealTime();
        playTimer->stop();
        break;
    case pausedImem:
        emit requestToStreamArchive();
        break;
    case pausedRtsp:
        emit requestToStreamRealTime();
        break;
    case stopped:
    default:
        emit requestToObtainSource(player->timeBar->firstVisibleTime, 1.0);
        break;
    }
}

/**
 * @brief PlayerController::testInputButtonClicked
 * Тестовый элемент управления перемоткой.
 */
void PlayerController::testInputButtonClicked()
{
    quint32 requestedTime = player->ui->testTimeInput->dateTime().toTime_t();
    emit requestToObtainSource(requestedTime, 1);
}

/**
 * @brief PlayerController::playRealTimeButtonClicked
 * Отправляет запрос на воспроизведение входного потока с камеры.
 */
void PlayerController::playRealTimeButtonClicked()
{
    emit requestToStreamRealTime();
}

/**
 * @brief PlayerController::speedUpButtonClicked
 * Отправляет запрос Дешинковщику на увеличение скорости воспроизведения.
 */
void PlayerController::speedUpButtonClicked()
{
    if(playbackState == PlaybackState::playingImem)
    {
        emit requestToSpeedUp();
    }
}

/**
 * @brief PlayerController::speedDownButtonClicked
 * Отправляет запрос Дешинковщику на уменьшение скорости воспроизведения.
 */
void PlayerController::speedDownButtonClicked()
{
    if(playbackState == PlaybackState::playingImem)
    {
        emit requestToSpeedDown();
    }
}

/**
 * @brief PlayerController::stepForwardButtonClicked
 * Отправляет запрос Дешинковщику перемотаться на фиксированный шаг вперёд.
 */
void PlayerController::stepForwardButtonClicked()
{
    if(playbackState == PlaybackState::playingImem)
    {
        emit requestToObtainSource(currentPlayTime + STEP_PERIOD, 1.0);
    }
}

/**
 * @brief PlayerController::stepBackButtonClicked
 * Отправляет запрос Дешинковщику перемотаться на фиксированный шаг назад.
 */
void PlayerController::stepBackButtonClicked()
{
    if(playbackState == PlaybackState::playingImem)
    {
        emit requestToObtainSource(currentPlayTime - STEP_PERIOD, 1.0);
    }
}

/**
 * @brief PlayerController::stopButtonClicked
 * Отправляет запрос на остановку обоих инстансов LibVLC.
 */
void PlayerController::stopButtonClicked()
{
    stopTimeSlider();
    emit requestToStop();
}

/**
 * @brief PlayerController::showSliderButtonClicked
 * Показывает участок таймбара, начинающийся с момента, который воспроизводится в данный момент.
 */
void PlayerController::showSliderButtonClicked()
{
    if(playbackState == PlaybackState::playingImem)
    {
        player->ui->dateTimeEdit->setDateTime(QDateTime::fromTime_t(currentPlayTime));
    }
}

/**
 * @brief PlayerController::updatePlaybackState
 * @param newState - новое состояние системы: что проигрывается и проигрывается ли вообще.
 * Изменяет состояние интерфейса в зависимости от состояния системы.
 */
void PlayerController::updatePlaybackState(PlaybackState newState)
{
    QIcon icon;
    switch (newState) {
    case playingImem:
        icon = QIcon(":/player/icons/pause");
        player->ui->buttonPlay->setIcon(icon);
        player->ui->buttonStop->setEnabled(true);
        player->ui->buttonStepBack->setEnabled(true);
        player->ui->buttonStepForward->setEnabled(true);
        player->ui->buttonSpeedDown->setEnabled(true);
        player->ui->buttonSpeedUp->setEnabled(true);
        break;
    case playingRtsp:
        setTimeLabelText("Камера");
        stopTimeSlider();
        icon = QIcon(":/player/icons/pause");
        player->ui->buttonPlay->setIcon(icon);
        player->ui->buttonStop->setEnabled(true);
        player->ui->buttonStepBack->setEnabled(false);
        player->ui->buttonStepForward->setEnabled(false);
        player->ui->buttonSpeedDown->setEnabled(false);
        player->ui->buttonSpeedUp->setEnabled(false);
        break;
    case pausedImem:
        setTimeLabelText("Камера");
        playTimer->stop();
        icon = QIcon(":/player/icons/play");
        player->ui->buttonPlay->setIcon(icon);
        player->ui->buttonStop->setEnabled(true);
        player->ui->buttonStepBack->setEnabled(false);
        player->ui->buttonStepForward->setEnabled(false);
        player->ui->buttonSpeedDown->setEnabled(false);
        player->ui->buttonSpeedUp->setEnabled(false);
        break;
    case pausedRtsp:
        setTimeLabelText("Камера");
        stopTimeSlider();
        icon = QIcon(":/player/icons/play");
        player->ui->buttonPlay->setIcon(icon);
        player->ui->buttonStop->setEnabled(true);
        player->ui->buttonStepBack->setEnabled(false);
        player->ui->buttonStepForward->setEnabled(false);
        player->ui->buttonSpeedDown->setEnabled(false);
        player->ui->buttonSpeedUp->setEnabled(false);
        break;
    case stopped:
    default:
        setTimeLabelText("Остановлено");
        stopTimeSlider();
        icon = QIcon(":/player/icons/playfirstvisible");
        player->ui->buttonPlay->setIcon(icon);
        player->ui->buttonStop->setEnabled(false);
        player->ui->buttonStepBack->setEnabled(false);
        player->ui->buttonStepForward->setEnabled(false);
        player->ui->buttonSpeedDown->setEnabled(false);
        player->ui->buttonSpeedUp->setEnabled(false);
        break;
    }
    playbackState = newState;
}
