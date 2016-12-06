#pragma once
//#include <vlc/vlc.h>
#include <include/vlc/vlc.h>

class Clean_Slicer
{
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
public:
    void makeSlicesFromStreamClean(const char* input, const char* output);
};
