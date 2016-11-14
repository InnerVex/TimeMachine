#ifndef PLAYERDEFINITIONS_H
#define PLAYERDEFINITIONS_H

struct SourceData {
    SourceData(std::string _path, long _sourceStartTime, long _sourceEndTime) :
        path(_path),
        sourceStartTime(_sourceStartTime),
        sourceEndTime(_sourceEndTime){}

    SourceData(){}

    std::string path;
    long sourceStartTime;
    long sourceEndTime;
}; 

#endif // PLAYERDEFINITIONS_H
