#pragma once
#include <QCoreApplication>

class TSHeader
{
    TSHeader();
public:

    bool TEI;
    bool PUSI;
    bool Priority;
    quint16 PID;
    unsigned TSC;
    unsigned AFC;
    unsigned CC;


    TSHeader(QDataStream &dataStream,QDataStream &outputStream);
    void Show();
};

class TSAdaptationField
{
public:
    quint8 length;
    bool Di, RAi, ESPi, PCRf, OPCRf, SPf, TPDf, AFEf;
    quint8 PCR[6], OPCR[6], Sc, TPDl;
    unsigned Sbl; //not native
    quint8 *TPD, *AE, *Sb;

    //Adaptation extension
    quint8 AEl;
    //TODO: rest of ae, it can be useful

    TSAdaptationField(QDataStream &dataStream);
    void Show();
};

class TSPayloadData
{
    QByteArray data;
public:
    //TSPayloadData(QDataStream datastream);
    void addData(quint8);
    void showData();
};

//not done
class TSPacket
{
    TSHeader *header;
public:
    TSPacket(QByteArray *buffer);
};

int Parse(const char* input, uint size_of_SLICE, const char* outputDst); //dont try this at home

