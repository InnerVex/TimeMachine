#include "streamcontroller.h"

#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <select.h>
#include <playerdefinitions.h>
#include <player.h>

#include <ui_player.h>

/**
 * @brief MyImemGetCallback
 * TODO::Описание входных параметров по ссылке
 * https://forum.videolan.org/viewtopic.php?t=93842
 */
int MyImemGetCallback (void *data,
                       const char *cookie,
                       int64_t *dts,
                       int64_t *pts,
                       unsigned *flags,
                       size_t * bufferSize,
                       void ** buffer)
{

    ImemData* imem = (ImemData*)data;
    cookie = imem->cookieString;
    if(imem == NULL || imem->videoPatches.size() == 0)
        return 1;

    //Указываем LibVLC на данные и их размер
    *buffer = (void*) (imem->videoPatches.front()).videoPatch;
    *bufferSize = (size_t) (imem->videoPatches.front()).bytes;
    imem->controller->totalBytesBuffered -= *bufferSize;
    imem->controller->replenishVideoPatchesBuffer();
    pop_front(imem->videoPatches);

    return 0;
}

/**
 * @brief MyImemReleaseCallback
 * TODO::Описание входных параметров по ссылке
 * https://forum.videolan.org/viewtopic.php?t=93842
 */
int MyImemReleaseCallback (void *data,
                           const char *cookie,
                           size_t bufferSize,
                           void * buffer)
{
    return 0;
}

/**
 * @brief StreamController::replenishVideoPatchesBuffer
 * Буферизует кусочки видео в оперативной памяти, пока не будет буферизовано VIDEO_PATCHES_TOTAL_BYTES байт.
 */
void StreamController::replenishVideoPatchesBuffer()
{
    //Цикл по файлам
    while(true)
    {
        bool eof = false;
        bool maxBuffered = false;

        //Цикл по видеопатчам
        while(true)
        {
            qint32 bytesToBuffer = VIDEO_PATCH_BYTES;
            if(currentFileBytePosition + bytesToBuffer > currentFileBytes)
            {
                bytesToBuffer = currentFileBytes - currentFileBytePosition;
                eof = true;
            }

            if(totalBytesBuffered > VIDEO_PATCHES_TOTAL_BYTES - VIDEO_PATCH_BYTES)
            {
                maxBuffered = true;
                break;
            }

            loadVideoPatchInMemory(bytesToBuffer);

            if(eof)
            {
                break;
            }
        }

        if(eof)
        {
            currentQFile->close();
            //Выбираем следующий файл
            currentFilename = Select::selectNextFile(currentFilename);
            if(QString::compare(currentFilename, "", Qt::CaseInsensitive) == 0)
            {
                //Архив закончился
                break;
            }
            openFileForBuffering(currentFilename);
        }

        if(maxBuffered)
        {
            break;
        }
    }
}

/**
 * @brief StreamController::openFileForBuffering
 * @param filename - имя файла.
 * @param withOffset - начинать буферизовать с начала или с байтового смещения, рассчитанного по временному.
 * Открывает файл для буферизации.
 */
void StreamController::openFileForBuffering(QString filename, bool withOffset)
{
    currentQFile = new QFile(filename);
    currentQFile->open(QIODevice::ReadOnly);
    currentFileBytes = currentQFile->size();

    //Рассчитываем первый байт для буфферизации
    if(withOffset)
    {
        currentFileBytePosition = (qint32)(currentFileBytes * percentOffset);
    }
    else
    {
        currentFileBytePosition = 0;
    }
}

/**
 * @brief StreamController::loadVideoPatchInMemory
 * @param bytesToBuffer - количество байт, которые нужно прочесть из файла и поместить в память.
 * Записывает часть файла в оперативную память и увеличивает значение сдвига по файлу.
 */
void StreamController::loadVideoPatchInMemory(qint32 bytesToBuffer)
{
    //Записываем файл в оперативную память
    char* buffer = (char*) malloc (bytesToBuffer);
    currentQFile->seek(currentFileBytePosition);
    qint64 result = currentQFile->read(buffer, bytesToBuffer);

    VideoPatchData data(buffer, result);
    mImemData->videoPatches.push_back(data);

    totalBytesBuffered += result;
    currentFileBytePosition += result;
}

/**
 * @brief StreamController::createImemInstance
 * Создаёт инстанс LibVLC для воспроизведения при помощи модуля Imem.
 * Здесь же создаёт медиа, плеер и прикрепляет плеер к окну.
 */
void StreamController::createImemInstance()
{
    //Задаём параметры LibVLC
    std::vector<const char*> options;
    std::vector<const char*>::iterator option;

    //Параметры для вывода стрима по rtsp
    //std::string soutLine = "--sout=#rtp{dst=localhost,port=5544,sdp=rtsp://localhost:5544/}";
    //options.push_back(soutLine.c_str());

    char imemDataArg[256];
    sprintf(imemDataArg, "--imem-data=%#p", mImemData);
    options.push_back(imemDataArg);

    char imemGetArg[256];
    sprintf(imemGetArg, "--imem-get=%#p", MyImemGetCallback);
    options.push_back(imemGetArg);

    char imemReleaseArg[256];
    sprintf(imemReleaseArg, "--imem-release=%#p", MyImemReleaseCallback);
    options.push_back(imemReleaseArg);

    options.push_back("--imem-cookie=timemachine");
    options.push_back("--imem-codec=undf");
    options.push_back("--imem-cat=4");
    options.push_back("--demux=ts");

    options.push_back("--no-video-title-show");
    options.push_back("--verbose=2");

    vlcInstanceImem = libvlc_new (int(options.size()), options.data());
    mediaImem = libvlc_media_new_location (vlcInstanceImem, "imem://");
    for(option = options.begin(); option != options.end(); option++)
    {
        libvlc_media_add_option(mediaImem, *option);
    }
    mediaPlayerImem = libvlc_media_player_new_from_media (mediaImem);

    imemInstanceReady = true;
}

/**
 * @brief StreamController::createRTPSInstance
 * Создаёт инстанс LibVLC для воспроизведения из внешнего источника по протоколу Rtsp
 * Здесь же создаёт медиа, плеер и прикрепляет плеер к окну.
 */
void StreamController::createRTPSInstance()
{
    //Задаём параметры LibVLC
    std::vector<const char*> options;
    options.push_back("--no-video-title-show");
    options.push_back("--verbose=2");

    vlcInstanceRtsp = libvlc_new (int(options.size()), options.data());
    QString source = "rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
    mediaRtsp = libvlc_media_new_location (vlcInstanceRtsp, source.toStdString().c_str());
    mediaPlayerRtsp = libvlc_media_player_new_from_media(mediaRtsp);

    rtspInstanceReady = true;
}

/**
 * @brief StreamController::StreamController
 * @param parent
 * Конструктор.
 */
StreamController::StreamController(Player *_player, QObject *parent) :
    QObject(parent),
    player(_player)
{
    mImemData = new ImemData();
    mImemData->cookieString = "TM_SC_Instance";
    mImemData->controller = this;
    attemptingToStartStream = false;
    streamingImem = false;
    imemInstanceReady = false;
    rtspInstanceReady = false;
    currentRate = 1;

    windid = player->ui->videoFrame->winId();

    //Создаём инстанс LibVLC для воспроизведения реалтайма
    if(!rtspInstanceReady)
    {
        createRTPSInstance();
    }
}

/**
 * @brief StreamController::requestedToObtainSource
 * @param _requestTime - Какой момент из архивного видео буферизовать
 * @param playSpeed - Deprecated
 * Начинает буферизацию архивного видео в оперативной памяти по запросу времени.
 */
void StreamController::requestedToObtainSource(quint32 _requestTime, float playSpeed)
{
    //Получение данных из БД и буфферизация
    requestTime = _requestTime;
    currentFilename = Select::selectFile(requestTime);
    percentOffset = Select::selectPercentOffset(requestTime);

    //Буфферизуем видеопатчи
    totalBytesBuffered = 0;
    mImemData->videoPatches.clear();
    openFileForBuffering(currentFilename, true);
    replenishVideoPatchesBuffer();

    //Создаём инстанс LibVLC для воспроизведения архивов
    if(!imemInstanceReady)
    {
        createImemInstance();
    }

    //Отправить сигнал о получении данных об источнике
    emit signalSourceObtained();
}

/**
 * @brief StreamController::startWaitingForStreamStart
 * @param isImem - Воспроизведение каким из инстансов LibVLC нужно проверять по таймеру.
 * Проверяет начало воспроизведения одним из инстансов,
 * чтобы отослать сигнал Проигрывателю на синхронизацию с GUI.
 */
void StreamController::startWaitingForStreamStart(bool isImem)
{
    //Дожидаемся начала стрима и отправляем сигнал проигрывателю
    libvlc_media_player_t *mediaPlayer = isImem ? mediaPlayerImem : mediaPlayerRtsp;

    mAttemptTimer = new QTimer();
    connect(mAttemptTimer, &QTimer::timeout, [=]()
    {
        if(libvlc_media_player_is_playing(mediaPlayer) != 0)
        {
            if(isImem)
            {
                emit signalTimerStart(requestTime);
            }
            //emit signalStreamStarted();
            attemptingToStartStream = false;
        }
        if(!attemptingToStartStream)
        {
            mAttemptTimer->stop();
        }
    });
    attemptingToStartStream = true;

    mAttemptTimer->start(START_SIGNAL_DELAY);
}

/**
 * @brief StreamController::requestedToStreamArchive
 * Начинает воспроизведение архивов при помощи модуля Imem.
 */
void StreamController::requestedToStreamArchive()
{
    if(streamingRtsp)
    {
        libvlc_media_player_stop(mediaPlayerRtsp);
        streamingRtsp = false;
    }

    libvlc_media_player_set_hwnd(mediaPlayerImem, (void*)windid );
    libvlc_media_player_play(mediaPlayerImem);
    streamingImem = true;

    startWaitingForStreamStart(true);
}

/**
 * @brief StreamController::requestedToStreamRealTime
 * Начинает воспроизведение видео из внешнего источника по протоколу Rtsp.
 */
void StreamController::requestedToStreamRealTime()
{
    if(streamingImem)
    {
        libvlc_media_player_stop(mediaPlayerImem);
        streamingImem = false;
    }

    libvlc_media_player_set_hwnd(mediaPlayerRtsp, (void*)windid );
    libvlc_media_player_play(mediaPlayerRtsp);
    streamingRtsp = true;

    startWaitingForStreamStart(false);
}


void StreamController::requestedToPauseStream()
{
    //TODO::Остановить стрим

}

/**
 * @brief StreamController::streamSpeedUp
 * Ускоряет воспроизведение. Сигнал на изменение значения скорости также посылается сигналом Проигрывателю.
 */
void StreamController::streamSpeedUp()
{
    if(streamingImem && imemInstanceReady)
    {
        if(currentRate < RATE_MAX)
        {
            currentRate *= RATE_MULTIPLY;
            libvlc_media_player_set_rate(mediaPlayerImem, currentRate);
            emit signalUpdateRate(currentRate);
            emit sendMessageToStatusBar("Current rate: " + QString::number(currentRate));
        }
    }
}

/**
 * @brief StreamController::streamSpeedDown Замедляет воспроизведение.
 * Замедляет воспроизведение. Сигнал на изменение значения скорости также посылается сигналом Проигрывателю.
 */
void StreamController::streamSpeedDown()
{
    if(streamingImem && imemInstanceReady)
    {
        if(currentRate > RATE_MIN)
        {
            currentRate /= RATE_MULTIPLY;
            libvlc_media_player_set_rate(mediaPlayerImem, currentRate);
            emit signalUpdateRate(currentRate);
            emit sendMessageToStatusBar("Current rate: " + QString::number(currentRate));
        }
    }
}
