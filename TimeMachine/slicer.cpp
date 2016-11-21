#include <slicer.h>
#include <QDebug>
#include <string>
#include <string.h>
#include <vlc/vlc.h>



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

    qDebug()<<"End of makeSlice";

}
