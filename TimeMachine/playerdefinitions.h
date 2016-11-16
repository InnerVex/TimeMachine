#pragma once

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
