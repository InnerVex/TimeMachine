#ifndef PLAYERDEFINITIONS_H
#define PLAYERDEFINITIONS_H

using namespace std;
struct SourceData {
    SourceData(string _path, long _sourceStartTime, long _sourceEndTime) :
        path(_path),
        sourceStartTime(_sourceStartTime),
        sourceEndTime(_sourceEndTime){}

    SourceData(){}

    string path;
    long sourceStartTime;
    long sourceEndTime;
};

#define SourceList QList<SourceData>

#endif // PLAYERDEFINITIONS_H
