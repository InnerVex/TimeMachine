#include "parser.h"
#include "QDebug"
#include "QDataStream"
#include "QFile"

void TSPayloadData::addData(quint8 byte)
{
    data.append(byte);
}

void TSPayloadData::showData()
{
    for(auto ch : data)
        qDebug()<<hex<<(quint8)ch;
}

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

TSHeader::TSHeader(QDataStream &dataStream,QDataStream &outputStream)
{
    quint8 byte;
    quint32 byte32=0;
    for (unsigned i=0;i<3;i++)
    {
        dataStream>>byte;
        outputStream<<byte;

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

TSPacket::TSPacket(QByteArray *buffer)
{
    QDataStream dataStream(buffer,QIODevice::ReadOnly);
    quint32 byte;
    dataStream>>byte;
    //header = new TSHeader(byte);
}


int Parse(const char* input, uint size_of_SLICE, const char* outputDst) //dont try this at home
{
    QFile file(input);
    if (!file.open(QIODevice::ReadOnly))
        return 0;

    QDataStream dataStream(&file);

    //quint32 byte;
    quint8 byte;
    uint i=0,k=0, t=0;
    uint pos_of_prev_PAT;
    bool flag=true;
    TSHeader *header;
    TSAdaptationField *af;
    TSPayloadData *pd;
    pd=new TSPayloadData;

    QByteArray output;
    QDataStream outputStream(&output,QIODevice::WriteOnly);

    //uint size_of_SLICE = 10000000;

    while(t<15000000 && flag)
    {
        dataStream>>byte;
        t++;
        outputStream<<byte; //not sure about proper time usage, should check about const size vs dynamic size
        //qDebug()<<t<<byte;
        //qDebug()<< (byte<<(8<<i));


        if((byte == 0x47) && ((t-1)%188==0))
        {
            //pd->showData();
            if(header!=nullptr)
                delete header;
            /*if (pd!=nullptr)
                delete pd;

            if (af!=nullptr)
                delete af;
                */

            //qDebug()<<i<<" "<<byte;
            header = new TSHeader(dataStream,outputStream);

            //header->Show();

            /*if (header->AFC==2 || header->AFC==3)
            {
                //dataStream>>byte;
                af = new TSAdaptationField(dataStream);
                //af->Show();
            }

            //if(header->AFC==1||header->AFC==3)
                pd = new TSPayloadData;

            */


            //qDebug()<<hex<<header->PID;

            if(header->PID == 0)
            {
                if (t<size_of_SLICE)
                    pos_of_prev_PAT = t - 1;
                else
                {
                    flag=false;
                }
                k++;
                qDebug()<<"New PAT"<<k<<i<<t<<(quint8)output[t-1];
                //flag = true;
            }
            /*if (flag)
            {
                if(header->PID==0x44)
                {
                    qDebug()<<"next video"<<i;
                    flag=false;
                }
                else
                    qDebug()<<hex<<header->PID;
            }*/
            //if(header->PID == 0x44)
                //qDebug()<<i;


            i++;
            t+=3; //that will work ONLY if only header is analyzed
        }
        /*else
        {
            if(header->AFC==1 || header->AFC==3)
            {
                pd->addData(byte);
            }
            //else
                //qDebug()<<"missed"<<(quint8)byte;
        }*/

        //i++;

    }

    qDebug()<<t<<pos_of_prev_PAT<<(quint8)output[pos_of_prev_PAT-1];

    QByteArray tmp;
    for(int j = pos_of_prev_PAT;j<t;j++)
        tmp.append(output[j]);

    output.chop(t - pos_of_prev_PAT); //be sure of accurate t
    qDebug()<<output.size()<<(quint8)output[output.size()-1];

    QFile ofile(outputDst);
    //QFile file("video\\output.ts");
    ofile.open(QIODevice::WriteOnly);

    QDataStream newStream(&ofile);

    for(int j=0;j<output.size();j++)
        newStream<<(quint8)output[j];
    qDebug()<<"Done";

    //TODO: recusion
}
