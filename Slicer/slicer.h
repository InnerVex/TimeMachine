#pragma once
#include <include/vlc/vlc.h>

class Slicer
{
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;
public:
    //Slicer();
    void convertToTS(const char* input, const char* output);
    void splitTS(const char* input); //depricated
    void parseTS(const char* input);
    void makeSlice(const char* input, int start, int stop, const char* output); //there's no guarantee that it will work fine from the first time
    int getDuration(const char* input);
    void convertToTsFromStream(const char* input, const char* output);
    void makeSliceFromStreamSmem();
    void makeSliceFromStreamDirty(const char* input, const char* output, int duration);
    void makeMultipleSlices(const char* input, int number);
};


