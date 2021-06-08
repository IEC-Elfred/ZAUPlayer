#include "MMAV.h"

extern "C" {
#include <libavcodec/packet.h>
}

MMAVPacket::MMAVPacket()
{
   pkt = av_packet_alloc();

}

MMAVPacket::~MMAVPacket()
{
    if (pkt != nullptr)
    {
        av_packet_free(&pkt);
        pkt = nullptr;
    }
}