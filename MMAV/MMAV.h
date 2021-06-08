
//在头文件中要用extern"C"扩起来ffmpeg的
extern "C"
{
    #include <libavcodec/packet.h>
    #include <libavformat//avformat.h>
}
class MMAVPacket
{
    public:
        AVPacket* pkt = nullptr;
        MMAVPacket();
        ~MMAVPacket();

};
class MMAVReader
{
    public:
        MMAVReader();
        ~MMAVReader();
        int Open(const char* path);
        int Close();
        int Read(MMAVPacket * packet);

    private:
        AVFormatContext* formatCtx = nullptr;
};