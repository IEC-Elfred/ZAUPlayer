#include "ZAUPlayer.h"
#include <iostream>

ZAUPlayerDecoderThread::ZAUPlayerDecoderThread() { decoder = new MMAVDecoder; }
ZAUPlayerDecoderThread::~ZAUPlayerDecoderThread() {
  if (decoder != nullptr) {
    delete decoder;
    decoder = nullptr;
  }
}

void ZAUPlayerDecoderThread::run() {
    int frameCount = 0;
    //进行解码操作
    while (!stopFlag) {
        MMAVPacket* pkt = nullptr;
        int ret = packetQueue.Pop(&pkt);
        if (ret) {
            //队列为空，取不到Packet
            continue;
        }

        decoder->SendPacket(pkt);
        while (1) {
            MMAVFrame frame;
            ret = decoder->RecvFrame(&frame);
            if (ret) {
                break;
            }
            frameCount++;
			printf("Decoder Success: %d\n", frameCount);
        }
        std::cout<<"Decoder end" << std::endl;
    }
    
}

int ZAUPlayerDecoderThread::Init(MMAVStream *avstream) {
  //进行解码操作
  return decoder->Init(avstream);
}
int ZAUPlayerDecoderThread::PutPacket(MMAVPacket *pkt) {
    return packetQueue.Push(pkt);
}

int ZAUPlayerDecoderThread::GetPacketQueueSize()
{
    return packetQueue.Size();
}
