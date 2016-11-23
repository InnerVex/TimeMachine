#include <slicer.h>
#include <QDebug>
#include <string>
#include <string.h>
#include <include/vlc/vlc.h>
#include <mutex>
//#include <QtTest/QTest>
#include <chrono>
#include <thread>


void Slicer::convertToTS(const char* input, const char* output)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=\"";
    const char* ending="\"}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    qDebug()<<param;
    const char * const vlc_args[] =
    {
        "--verbose=2",
        param,
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\build-Slicer-Desktop_Qt_5_7_0_MinGW_32bit-Debug\\debug\\video\\output.ts\"}",
        //"--sout-transcode-deinterlace",
        //"--packetizer-mpegvideo-sync-iframe",
        //"--sout-ts-use-key-frames",
        //"--sout-ts-shaping=50",
    };


    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_path (inst, input);
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);
}

void Slicer::splitTS(const char* input)
{
    uint start=0;
    uint interval=120;
    uint n=0;
    while(n<1)
    {
        std::string start_time = "--start-time="; // C magic of editing const char* const
        start_time+=std::to_string(n*interval);
        char *param1;
        param1 = new char[start_time.length()];
        strcpy(param1,start_time.data());

        qDebug()<<param1;

        std::string stop_time = "--stop-time=";
        stop_time+=std::to_string((n+1)*interval);
        char *param2;
        param2 = new char[stop_time.length()];
        strcpy(param2,stop_time.data());

        qDebug()<<param2;

        std::string sout = "--sout=#file{dst=\"D:\\Work\\TD\\test\\Projects\\build-Slicer-Desktop_Qt_5_7_0_MinGW_32bit-Debug\\debug\\video\\slices\\slice_";  //there is no mux now
        sout+=std::to_string(n)+".ts\"}";                                                                                                                     //so input file must be .ts
        char *param3;
        param3 = new char[sout.length()];
        strcpy(param3,sout.data());

        const char * const vlc_args[] =
        {
            //"--verbose=2",
            param1,
            param2,
            param3,
            "vlc://quit",
        };

        inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
        m = libvlc_media_new_path (inst, input);
        mp = libvlc_media_player_new_from_media (m);
        libvlc_media_player_play(mp);
        //delete param1; //there probably should be some cleaning but mistakes were made
        //delete param2;
        //delete param3;
        qDebug()<<"End of cycle"<<n;
        n++;

        int st;
    }
}

void Slicer::makeSlice(const char* input, int start, int stop, const char* output)
{
    std::string start_time = "--start-time="; // concatination of str and int #Idummy
    start_time+=std::to_string(start);
    char *param1;
    param1 = new char[start_time.length()];
    strcpy(param1,start_time.data());

    std::string stop_time = "--stop-time=";
    stop_time+=std::to_string(stop);
    char *param2;
    param2 = new char[stop_time.length()];
    strcpy(param2,stop_time.data());

    const char* sout="--sout=#file{dst=\"";
    const char* ending="\"}";
    char *param3;
    param3 = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param3,sout),output);
    strcat(param3,ending);

    qDebug()<<param1<<param2<<param3;


    const char * const vlc_args[] =
    {
        "--verbose=2",
        param1,
        param2,
        param3,
        "vlc://quit",
    };

    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_path (inst, input);
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    qDebug()<<"End of makeSlice";

}

int Slicer::getDuration(const char* input)
{
    const char * const vlc_args[] =
    {
        //"--verbose=2",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_path (inst, input);
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_parse(m);
    return libvlc_media_get_duration(m);
}

void Slicer::convertToTsFromStream(const char* input, const char* output)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=\"";
    const char* ending="\"}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    const char * const vlc_args[] =
    {
        "--verbose=2",
        param,
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,input);  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    //QTest::qSleep(10000);

    //std::this_thread::sleep_for(std::chrono::milliseconds(30000));
    //qDebug()<<"Hi";

    //libvlc_media_player_stop(mp);
}

void Slicer::makeMultipleSlices(const char* input, int number)
{
    char output[1000];
    int duration_of_slice = 20000; // in milliseconds
    for (long long int i=0;i<number;i++)
    {
        sprintf(output
                ,"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream_%lld"
                ".ts"
                ,i
                );
        qDebug()<<output;
        this->makeSliceFromStreamDirty(input,output,duration_of_slice);
    }
}

void Slicer::makeSliceFromStreamDirty(const char *input, const char *output, int duration)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=\"";
    const char* ending="\"}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    const char * const vlc_args[] =
    {
        "--verbose=2",
        param,
        //"--run-time=5",
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,input);  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    //QTest::qSleep(10000);

    std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    qDebug()<<"Hi";

    libvlc_media_player_stop(mp);
}


std::mutex imageMutex;
uint8_t * videoBuffer;
void cbVideoPrerender(void *p_video_data, uint8_t **pp_pixel_buffer, int size) {
    // Locking
    imageMutex.lock();
    qDebug()<<(dec)<<"Size is"<<size<<(int)p_video_data;
    videoBuffer = (uint8_t *)malloc(size);
    *pp_pixel_buffer = videoBuffer;
}
void cbVideoPostrender(void *p_video_data, uint8_t *p_pixel_buffer
      , int width, int height, int pixel_pitch, int size, int64_t pts) {
   // Unlocking
   imageMutex.unlock();
}

void Slicer::makeSliceFromStreamSmem()
{
    char smem_options[1000];
       sprintf(smem_options
          , "#smem{"
             "video-prerender-callback=%lld,"
             "video-postrender-callback=%lld}"
          , (long long int)(intptr_t)(void*)&cbVideoPrerender
          , (long long int)(intptr_t)(void*)&cbVideoPostrender //This would normally be useful data, 100 is just test data
         // , "D:\\Work\\TD\\test\\Projects\\video\\example1.flv" //Test data
          );
       //strcat(smem_options,"D:\\Work\\TD\\test\\Projects\\video\\example1.flv}");

       strcat(smem_options,":std{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromOutput.ts\"}");



        qDebug()<<smem_options;

    const char * const vlc_args[] =
    {
        "-I", "dummy", // Don't use any interface
        "--ignore-config", // Don't use VLC's config
        //"--extraintf=logger", // Log anything
        "--verbose=2", // Be verbose
        "--sout", smem_options // Stream to memory
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    //mp = libvlc_media_player_new(inst);

    m = libvlc_media_new_path(inst,"D:\\Work\\TD\\test\\Projects\\video\\example1.flv");  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);
}
