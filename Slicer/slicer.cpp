#include <slicer.h>
#include <QDebug>
#include <string>
#include <string.h>
#include <vlc/vlc.h>
#include <mutex>
//#include <QtTest/QTest>
#include <chrono>
#include <thread>
#include <pipe_server.h>
#include <socket_server.h>
#include <QThread>

void Slicer::makeSlicesUsingSocket(const char *input, const char *output_location, int number)
{
    /*
    //Socket_Server server;
    //QThread serv(Socket_Server::start_Server);
    std::thread serv(Socket_Server::start_Server);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::thread writer(Socket_Server::write);
    //std::thread reader(Socket_Server::read);

    writer.join();
    //reader.join();
    serv.join();
    */

    Server server;
    server.show();
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

void Slicer::makeSlicesUsingPipe(const char *input, const char *output_location, int number)
{
    Pipe_Server pipe;
    std::thread server(Pipe_Server::createPipe);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //std::thread writer(Pipe_Server::writeToPipe);
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::thread reader(Pipe_Server::readFromPipe);
    server.join();
    //writer.join();
    reader.join();
    //pipe.createPipe();
}

void Slicer::convertToTS(const char* input, const char* output)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=";
    const char* ending="}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    qDebug()<<param;
    const char * const vlc_args[] =
    {
        //"--verbose=2",
        param,
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\build-Slicer-Desktop_Qt_5_7_0_MinGW_32bit-Debug\\debug\\video\\output.ts\"}",
        //"--sout-transcode-deinterlace",
        //"--packetizer-mpegvideo-sync-iframe",
        //"--sout-ts-use-key-frames",
        //"--sout-ts-shaping=50",
        "vlc://quit",
    };

    libvlc_instance_t *inst_tmp;
    libvlc_media_player_t *mp_tmp;
    libvlc_media_t *m_tmp;

    inst_tmp=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m_tmp = libvlc_media_new_path(inst_tmp, input);
    mp_tmp = libvlc_media_player_new_from_media(m_tmp);
    libvlc_media_player_play(mp_tmp);
    qDebug()<<"Hi1";
    std::this_thread::sleep_for(std::chrono::milliseconds(10000)); //TODO: Event catcher! or there is waste of time
    qDebug()<<"Hi2";
    libvlc_media_player_stop(mp_tmp);
}

void Slicer::splitTS(const char* input)
{
    uint start=0;
    uint interval=120;
    uint n=0;
    while(n<1)
    {
        std::string start_time = "--start-time="; // C magic of editing const char* const
        start_time+=std::to_string(n*interval);
        char *param1;
        param1 = new char[start_time.length()];
        strcpy(param1,start_time.data());

        qDebug()<<param1;

        std::string stop_time = "--stop-time=";
        stop_time+=std::to_string((n+1)*interval);
        char *param2;
        param2 = new char[stop_time.length()];
        strcpy(param2,stop_time.data());

        qDebug()<<param2;

        std::string sout = "--sout=#file{dst=\"D:\\Work\\TD\\test\\Projects\\build-Slicer-Desktop_Qt_5_7_0_MinGW_32bit-Debug\\debug\\video\\slices\\slice_";  //there is no mux now
        sout+=std::to_string(n)+".ts\"}";                                                                                                                     //so input file must be .ts
        char *param3;
        param3 = new char[sout.length()];
        strcpy(param3,sout.data());

        const char * const vlc_args[] =
        {
            //"--verbose=2",
            param1,
            param2,
            param3,
            "vlc://quit",
        };

        inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
        m = libvlc_media_new_path (inst, input);
        mp = libvlc_media_player_new_from_media (m);
        libvlc_media_player_play(mp);
        //delete param1; //there probably should be some cleaning but mistakes were made
        //delete param2;
        //delete param3;
        qDebug()<<"End of cycle"<<n;
        n++;

        int st;
    }
}

void Slicer::makeSlice(const char* input, int start, int stop, const char* output)
{
    std::string start_time = "--start-time="; // concatination of str and int #Idummy
    start_time+=std::to_string(start);
    char *param1;
    param1 = new char[start_time.length()];
    strcpy(param1,start_time.data());

    std::string stop_time = "--stop-time=";
    stop_time+=std::to_string(stop);
    char *param2;
    param2 = new char[stop_time.length()];
    strcpy(param2,stop_time.data());

    const char* sout="--sout=#file{dst=\"";
    const char* ending="\"}";
    char *param3;
    param3 = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param3,sout),output);
    strcat(param3,ending);

    qDebug()<<param1<<param2<<param3;


    const char * const vlc_args[] =
    {
        "--verbose=2",
        param1,
        param2,
        param3,
        "vlc://quit",
    };

    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_path (inst, input);
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    qDebug()<<"End of makeSlice";

}

int Slicer::getDuration(const char* input)
{
    const char * const vlc_args[] =
    {
        //"--verbose=2",
        //"-I","dummy",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_path (inst, input);
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_parse(m);
    //libvlc_media_player_play(mp);
    //libvlc_media_player_stop(mp);

    //libvlc_video_get_
    return libvlc_media_get_duration(m);
    //return libvlc_media_player_get_length(mp);
}

void Slicer::convertToTsFromStream(const char* input, const char* output)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=\"";
    const char* ending="\"}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    const char * const vlc_args[] =
    {
        //"--verbose=2",
        param,
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,input);  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    //QTest::qSleep(10000);

    //std::this_thread::sleep_for(std::chrono::milliseconds(30000));
    //qDebug()<<"Hi";

    //libvlc_media_player_stop(mp);
}

void Slicer::makeMultipleSlices(const char* input, const char* output_location, int number)
{
    char tmp_output[1000];
    char final_output[1000];
    char tmp[100];
    //int duration_of_slice = 20000; // in milliseconds //currently only holding the thread
    int duration_of_slice = 20; //in sec
    for (long long int i=0;i<number;i++)
    {
        sprintf(tmp_output
                ,output_location //"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream_%lld"
                //"\\tmp.ps"
                //".ps"
                //,i
                );
        sprintf(tmp
                ,"\\tmp_%lld"  //"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream_%lld"
                ".ps"
                ,i
                );
        strcat(tmp_output,tmp); //"\\tmp.ps");
        qDebug()<<tmp_output;
        this->makeSliceFromStreamDirty(input,tmp_output,duration_of_slice);

        sprintf(final_output
                ,output_location);

        sprintf(tmp
                ,"\\Slice_%lld"  //"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream_%lld"
                ".ts"
                ,i
                );
        strcat(final_output,tmp);
        qDebug()<<final_output;
        std::thread t1;
        t1 = std::thread(convertToTS,this,tmp_output,final_output);
        t1.detach();
        //this->convertToTS(tmp_output,final_output);
        qDebug()<<"Duration of"<<final_output<<"is"<<getDuration(tmp_output); //currently not working with stream
        qDebug()<<"Slice made"<<i;
    }
}

void Slicer::makeSliceFromStreamDirty(const char *input, const char *output, int duration)
{
    const char* sout="--sout=#standard{access=file,mux=ts,dst=\\\\";
    //const char* sout="--sout=#file{mux=ps,dst=";
    const char* ending="}";
    char *param;
    param = new char[strlen(sout)+strlen(output)+strlen(ending)];
    strcat(strcpy(param,sout),output);
    strcat(param,ending);

    qDebug()<<param;
    std::string stop_time = "--run-time=";
    stop_time+=std::to_string(duration);
    char *param2;
    param2 = new char[stop_time.length()];
    strcpy(param2,stop_time.data());

    qDebug()<<param2;

    const char * const vlc_args[] =
    {
        "--verbose=2",
        param,
        //"--sout=#standard{access=file,mux=ts,dst=\\\\\\\\.\\pipe\\my_soket}",
        param2,
        //"--live-caching",
        //"--network-caching",
        //"--run-time=10", //run time stared working well, so should use this instead
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,input);  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);

    //QTest::qSleep(10000);

    //std::this_thread::sleep_for(std::chrono::milliseconds((duration)*1500)); //TODO: Event catcher! or there might be some problems with threads
    qDebug()<<"Hi";

    //libvlc_media_player_stop(mp);
    qDebug()<<"Hi2";
    //libvlc_release(inst);
}


void Slicer::testFunc()
{
    /*const char * const vlc_args[] =
    {
        "--verbose=2",
        //"--live-caching=2000",
        //"--network-caching",
        //"--run-time=5",
        //"--sout-mux-caching=100",
        //"--network-caching=4000",
        //"--sout-ts-dts-delay=30000",
        //"--ts-seek-percent",
        //"--sout=#standard{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}",
        "rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp",
        "--sout=#file{mux=mp4,dst=\"D:\\example\\outputFromStream.mp4\"}",
        //"--sout=#transcode{}:duplicate{dst=std{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}}",
        //":sout-keep"
        "--run-time=30",
        //"--stop-time=10",
        "vlc://quit",
    };*/
    const char * const vlc_args[] =
    {
        "--verbose=2",
        //"--live-caching=4000",
        "--sout=#file{mux=ps,dst=D:\\example\\outputFromStream.ts}",
        "--run-time=10",
        "vlc://quit"
    };

    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    m = libvlc_media_new_location(inst,"rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp");  //"rtsp://mpv.cdn3.bigCDN.com:554/bigCDN/definst/mp4:bigbuckbunnyiphone_400.mp4");//"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    mp = libvlc_media_player_new_from_media (m);
    //mp = libvlc_media_player_new(inst);
    libvlc_media_player_play(mp);

}


std::mutex imageMutex;
uint8_t * videoBuffer;
void cbVideoPrerender(void *p_video_data, uint8_t **pp_pixel_buffer, int size) {
    // Locking
    imageMutex.lock();
    qDebug()<<(dec)<<"Size is"<<size<<(int)p_video_data;
    videoBuffer = (uint8_t *)malloc(size);
    *pp_pixel_buffer = videoBuffer;
}
void cbVideoPostrender(void *p_video_data, uint8_t *p_pixel_buffer
      , int width, int height, int pixel_pitch, int size, int64_t pts) {
    qDebug()<<(dec)<<"Postrender"<<size<<pts<<p_video_data;
   // Unlocking
   imageMutex.unlock();
}

void Slicer::makeSliceFromStreamSmem()
{
    char smem_options[1000];
       sprintf(smem_options
          , "#transcode{vcodec=RV24,acodec=s16l}:smem{"
             "video-prerender-callback=%lld,"
             "video-postrender-callback=%lld}"
          , (long long int)(intptr_t)(void*)&cbVideoPrerender
          , (long long int)(intptr_t)(void*)&cbVideoPostrender //This would normally be useful data, 100 is just test data
         // , "D:\\Work\\TD\\test\\Projects\\video\\example1.flv" //Test data
          );
       //strcat(smem_options,"D:\\Work\\TD\\test\\Projects\\video\\example1.flv}");

       strcat(smem_options,":std{access=file,mux=ts,dst=\"D:\\Work\\TD\\test\\Projects\\video\\outputFromStream.ts\"}");



        qDebug()<<smem_options;

    const char * const vlc_args[] =
    {
        "-I", "dummy", // Don't use any interface
        "--ignore-config", // Don't use VLC's config
        //"--extraintf=logger", // Log anything
        "--verbose=2", // Be verbose
        "--sout", smem_options, // Stream to memory
        "--run-time=30",
        "--stop-time=30",
        "vlc://quit",
    };
    inst=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

    //mp = libvlc_media_player_new(inst);

    //m = libvlc_media_new_path(inst,//"D:\\Work\\TD\\test\\Projects\\video\\example1.flv");  //"http://ar.solumedia.com.ar:1935/cool/hd/playlist.m3u8"
    m = libvlc_media_new_location(inst,"rtsp://ewns-hls-b-stream.hexaglobe.net/rtpeuronewslive/en_vidan750_rtp.sdp");  //"rtsp://mpv.cdn3.bigCDN.com:554/bigCDN/definst/mp4:bigbuckbunnyiphone_400.mp4");
    mp = libvlc_media_player_new_from_media (m);
    libvlc_media_player_play(mp);
}
