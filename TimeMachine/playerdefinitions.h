#pragma once

#define PLAY_ATTEMPT_DELAY 2000

struct SourceData {
    SourceData(std::string _path, qint32 _sourceStartTime, qint32 _sourceEndTime) :
        path(_path),
        sourceStartTime(_sourceStartTime),
        sourceEndTime(_sourceEndTime){}

    SourceData(){}

    std::string path;
    long sourceStartTime;
    long sourceEndTime;
}; 

struct VideoPatchData
{
    VideoPatchData(){}
    VideoPatchData(const VideoPatchData &_data) : videoPatch(_data.videoPatch), bytes(_data.bytes) {}
    VideoPatchData(char* _videoPatch, qint32 _bytes) : videoPatch(_videoPatch), bytes(_bytes) {}
    char* videoPatch;
    qint32 bytes;
};
