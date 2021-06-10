#include "ZAUPlayer.h"
#include <iostream>

ZAUPlayerDecoderThread::ZAUPlayerDecoderThread(ZAUPlayerController *_playerCtr,
                                               ZAUDecoderType _type) {
  decoder = new MMAVDecoder;
  playerCtr = _playerCtr;
  type = _type;
}
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
    if (type == ZAUDecoderType ::ZAUDECODER_TYPE_VIDEO) {
      if (playerCtr->GetVideoQueueSize() > 10) {
        continue;
      }
    }
    if (type == ZAUDecoderType ::ZAUDECODER_TYPE_AUDIO) {
      if (playerCtr->GetAudioQueueSize() > 20) {
        continue;
      }
    }
    MMAVPacket *pkt = nullptr;
    int ret = packetQueue.Pop(&pkt);
    if (ret) {
      //队列为空，取不到Packet,继续自旋
      continue;
    }
    

    decoder->SendPacket(pkt);
    while (1) {
      MMAVFrame *frame = new MMAVFrame;
      ret = decoder->RecvFrame(frame);
      if (ret) {
        break;
      }
      frameCount++;
      //通过playerCtr句柄，向其塞入Frame数据
      if (type == ZAUDecoderType ::ZAUDECODER_TYPE_VIDEO) {
        playerCtr->PushFrameToVideoQueue(frame);
      }
      if (type == ZAUDecoderType ::ZAUDECODER_TYPE_AUDIO) {
        playerCtr->PushFrameToAudioQueue(frame);
      }
    }
  }
}

int ZAUPlayerDecoderThread::Init(MMAVStream *avstream) {
  //进行解码操作
  return decoder->Init(avstream);
}
int ZAUPlayerDecoderThread::PutPacket(MMAVPacket *pkt) {
  return packetQueue.Push(pkt);
}

int ZAUPlayerDecoderThread::GetPacketQueueSize() { return packetQueue.Size(); }
