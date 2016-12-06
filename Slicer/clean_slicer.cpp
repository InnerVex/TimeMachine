#include "clean_slicer.h"
#include <QDebug>

void Clean_Slicer::makeSlicesFromStreamClean(const char *input, const char *output, int duration)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=\\\\"; //not sure
    //const char* sout="--sout=#file{mux=ps,dst=";
    const char* ending="}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    qDebug()<<param;
    std::string stop_time = "--run-time=";
    stop_time+=std::to_string(duration);
    char *param2;
    param2 = new char[stop_time.length()];
    strcpy(param2,stop_time.data());

    qDebug()<<param2;

    const char * const vlc_args[] =
    {
        //"--verbose=2",
        param,
        param2,
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,input);  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    qDebug()<<"Hi2";
}
