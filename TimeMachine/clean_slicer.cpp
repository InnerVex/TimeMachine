#include "clean_slicer.h"
#include <QDebug>
#include <QtGlobal>

void Clean_Slicer::makeSlicesFromStreamClean(const char *input, const char *output)
{
    const char* sout="--sout=#standard{access=udp{caching=3000},mux=ts,dst="; //unix
    const char* ending="}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    const char * const vlc_args[] =
    {
        //"--verbose=2",
        param,
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,input);  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    qDebug()<<"VLC started";
}

int Clean_Slicer::getDuration(const char *input)
{
    const char * const vlc_args[] =
    {
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_path (inst, input);
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_parse(m);

    return libvlc_media_get_duration(m);
}
