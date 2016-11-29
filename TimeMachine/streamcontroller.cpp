#include "streamcontroller.h"

#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include "select.h"

//Get и Release коллбэки для IMEM
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
    pop_front(imem->videoPatches);

    //TODO::Снова запускаем буфферизацию, уменьшаем buffered bytes

    return 0;
}

int MyImemReleaseCallback (void *data,
                           const char *cookie,
                           size_t bufferSize,
                           void * buffer)
{
    return 0;
}

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

            loadVideoPatchInMemory(bytesToBuffer);

            if(totalBytesBuffered > VIDEO_PATCHES_TOTAL_BYTES - VIDEO_PATCH_BYTES)
            {
                maxBuffered = true;
                break;
            }

            if(eof)
            {
                break;
            }
        }

        if(eof)
        {
            fclose(currentFile);
            //TODO::Получаем имя следующего файла
            openFileForBuffering(currentFilename);
        }

        if(maxBuffered)
        {
            break;
        }
    }
}

void StreamController::openFileForBuffering(const char* filename, bool withOffset)
{
    currentFile = fopen(currentFilename,"rb");

    //Определяем размер файла в байтах
    fseek(currentFile,0,SEEK_END);
    currentFileBytes = ftell(currentFile);
    rewind (currentFile);

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

void StreamController::loadVideoPatchInMemory(qint32 bytesToBuffer)
{
    //Записываем файл в оперативную память
    char* buffer = (char*) malloc (bytesToBuffer);
    fseek(currentFile, currentFileBytePosition, SEEK_SET);
    size_t result = fread (buffer, 1, bytesToBuffer, currentFile);

    VideoPatchData data(buffer, (qint32)result);
    mImemData->videoPatches.push_back(data);

    totalBytesBuffered += result;
    currentFileBytePosition += result;
}

StreamController::StreamController(QObject *parent) : QObject(parent)
{
    mImemData = new ImemData();
    mImemData->cookieString = "TM_SC_Instance";
    attemptingToStartStream = streaming = false;
}

/**
 * Данная функция осуществляет остановку и пересоздание инстанса LibVLC
 *
 * @brief StreamController::requestedToObtainSource
 * @param requestTime
 * @param playSpeed
 */
void StreamController::requestedToObtainSource(quint32 requestTime, float playSpeed)
{
    if(streaming)
    {
        libvlc_media_player_stop(mMediaPlayer);
        streaming = false;
    }

    //Получение данных из БД и буфферизация
    currentFilename = Select::selectFile(requestTime).toStdString().c_str();
    percentOffset = Select::selectPercentOffset(requestTime);

    //Буфферизуем видеопатчи
    totalBytesBuffered = 0;
    mImemData->videoPatches.clear();
    openFileForBuffering(currentFilename, true);
    //openFileForBuffering(currentFilename, false);
    replenishVideoPatchesBuffer();

    //Задаём параметры LibVLC
    std::vector<const char*> options;
    std::vector<const char*>::iterator option;

    std::string soutLine = "--sout=#rtp{dst=localhost,port=5544,sdp=rtsp://localhost:5544/}";
    options.push_back(soutLine.c_str());

    options.push_back("--no-video-title-show");

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
    options.push_back("--verbose=2");
    options.push_back("--demux=ts");

    mVlcInstance = libvlc_new (int(options.size()), options.data());
    mMedia = libvlc_media_new_location (mVlcInstance, "imem://");
    mMediaPlayer = libvlc_media_player_new_from_media (mMedia);

    for(option = options.begin(); option != options.end(); option++)
    {
        libvlc_media_add_option(mMedia, *option);
    }

    //Отправить сигнал о получении данных об источнике
    emit signalSourceObtained();
}

void StreamController::startWaitingForStreamStart()
{
    //Дожидаемся начала стрима и отправляем сигнал проигрывателю
    mAttemptTimer = new QTimer();
    connect(mAttemptTimer, &QTimer::timeout, [=]()
    {
        if(libvlc_media_player_is_playing(mMediaPlayer) != 0)
        {
            emit signalStreamStarted();
            attemptingToStartStream = false;
        }
        if(!attemptingToStartStream)
        {
            mAttemptTimer->stop();
        }
    });
    attemptingToStartStream = true;
    mAttemptTimer->start(1000);
}

void StreamController::requestedToStream(float playSpeed)
{
    libvlc_media_player_play(mMediaPlayer);
    streaming = true;

    startWaitingForStreamStart();
}
void StreamController::requestedToStreamRealTime()
{
    //TODO::Сюда добавить код создания инстанса LibVLC
    QString source = "rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp";
    currentFilename = source.toStdString().c_str();
    mMedia = libvlc_media_new_location(mVlcInstance, currentFilename);
    libvlc_media_player_set_media (mMediaPlayer, mMedia);
    libvlc_media_player_play(mMediaPlayer);

    startWaitingForStreamStart();
}
void StreamController::requestedToPauseStream()
{
    //TODO::Остановить стрим

}
