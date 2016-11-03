#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QByteArray>

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


    TSHeader(QDataStream &dataStream);
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
};

TSAdaptationField::TSAdaptationField(QDataStream &dataStream)
{
    quint8 byte;
    dataStream>>byte;
    length = byte;

    Sbl = length;

    dataStream>>byte;
    Di = byte & 0x80;
    RAi = byte & 0x40;
    ESPi = byte & 0x20;
    PCRf = byte & 0x10;
    OPCRf = byte & 0x08;
    SPf = byte & 0x04;
    TPDf = byte & 0x02;
    AFEf = byte & 0x01;

    Sbl--;

    if (PCRf)
    {
        for(int i =0;i<6;i++)
        {
            dataStream>>byte;
            PCR[i]=byte;
            Sbl--;
        }
    }

    if (OPCRf)
    {
        for(int i =0;i<6;i++)
        {
            dataStream>>byte;
            OPCR[i]=byte;
            Sbl--;
        }
    }

    if (SPf)
    {
        dataStream>>byte;
        Sc = byte;
        Sbl--;
    }

    if (TPDf)
    {
        dataStream>>byte;
        TPDl = byte;
        Sbl--;

        TPD = new quint8[TPDl];

        for(int i=0;i<TPDl;i++)
        {
            dataStream>>byte;
            TPD[i]=byte;
            Sbl--;
        }
    }

    if (AFEf)
    {
        dataStream>>byte;
        AEl=byte;
        Sbl--;
        //TODO: rest of ae, probably as another func
        Sbl-=byte;//because there is no realisation, can left it like that
    }

    Sb = new quint8[Sbl];
    for (int i=0;i<Sbl;i++)
    {
        dataStream>>byte;
        Sb[i]=byte;
    }

}

void TSAdaptationField::Show()
{
    qDebug()<<"Length of AF"<< length;
    qDebug()<<"Parameters of AF:"<< Di<<RAi<<ESPi<<PCRf<<OPCRf<<SPf<<TPDf<<AFEf;
    qDebug()<<Sbl<<Sb[0]<<Sb[Sbl-1];
}

TSHeader::TSHeader(QDataStream &dataStream)
{
    quint8 byte;
    quint32 byte32=0;
    for (unsigned i=0;i<3;i++)
    {
        dataStream>>byte;
        byte32+=byte<<(8*(2-i));
    }
    TEI = byte32 & 0x800000;
    PUSI = byte32 & 0x400000;
    Priority = byte32 & 0x200000;
    PID = (byte32 & 0x1ff00)>>8;
    TSC = (byte32 & 0xc0)>>6;
    AFC = (byte32 & 0x30)>>4;
    CC = (byte32 & 0xf);
}

void TSHeader::Show()
{
    qDebug()<<"Transport Error Indicator :"<<TEI;
    qDebug()<<"Payload Unit Start Indicator :"<<PUSI;
    qDebug()<<"Transport Priority :"<<Priority;
    qDebug()<<"PID :"<<PID;
    qDebug()<<"Transport Scrambling Control :"<<TSC;
    qDebug()<<"Adaptation field control :"<<AFC;
    qDebug()<<"Continuity counter :"<<CC<<"\n";
}

class TSPacket
{
    TSHeader *header;
public:
    TSPacket(QByteArray *buffer);
};

TSPacket::TSPacket(QByteArray *buffer)
{
    QDataStream dataStream(buffer,QIODevice::ReadOnly);
    quint32 byte;
    dataStream>>byte;
    //header = new TSHeader(byte);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("D:\\Work\\TD\\test\\Projects\\TestLibVLC\\video\\output.ts");
    //QFile file("video\\output.ts");
    file.open(QIODevice::ReadOnly);

    QDataStream dataStream(&file);

    //quint32 byte;
    quint8 byte;
    uint i=0;
    TSHeader *header;
    TSAdaptationField *af;

    while(i<10)
    {
        dataStream>>byte;

        //qDebug()<< byte;
        //qDebug()<< (byte<<(8<<i));

        if(byte == 0x47)
        {
            //qDebug()<<i<<" "<<byte;
            header = new TSHeader(dataStream);
            header->Show();
            if (header->AFC==2 || header->AFC==3)
            {
                //dataStream>>byte;
                af = new TSAdaptationField(dataStream);
                af->Show();
            }

            //qDebug()<<"New "<<(byte & 0x800000)<<" "<<(byte & 0x400000)<<" "<<(byte & 0x200000)<<" "<<(byte & 0x1ff00)<<" "<<(byte & 0xc0)<<" "<<(byte & 0x30)<<" "<<(byte & 0xf);
            //qDebug()<<((byte & 0x30)==0x40)<<" "<<((byte & 0x30)==0x80)<<" "<<((byte & 0x30)==0xc0);
        }
        i++;
    }

    return a.exec();
}
