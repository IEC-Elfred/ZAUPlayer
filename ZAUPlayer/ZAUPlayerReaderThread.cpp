#include "ZAUPlayer.h"
#include <iostream>

ZAUPlayerReaderThread::ZAUPlayerReaderThread(std::string _path) {
  path = _path;
}

ZAUPlayerReaderThread::~ZAUPlayerReaderThread() {}

void ZAUPlayerReaderThread::run() {
  MMAVReader reader;
  int ret = reader.Open(path.c_str());
  if (ret) {
    return;
  }
  int videoStreamIndex = reader.GetVideoStreamIndex();
  int audioStreamIndex = reader.GetAudioStreamIndex();
  ZAUPlayerDecoderThread *videoDecoderThread = new ZAUPlayerDecoderThread();
  ZAUPlayerDecoderThread *audioDecoderThread = new ZAUPlayerDecoderThread();

  //初始化解码器
  if (videoStreamIndex >= 0) {

    MMAVStream videoStream;
    reader.GetStream(&videoStream, videoStreamIndex);
    videoDecoderThread->Init(&videoStream);
    videoDecoderThread->Start();
  }
  if (audioStreamIndex >= 0) {
    MMAVStream audioStream;
    reader.GetStream(&audioStream, audioStreamIndex);
    audioDecoderThread->Init(&audioStream);
    audioDecoderThread->Start();
  }

  while (!stopFlag) {
    if (videoDecoderThread->GetPacketQueueSize() > 5 ||
        audioDecoderThread->GetPacketQueueSize() > 5) {
      continue;
    }

    MMAVPacket *pkt = new MMAVPacket();
    int ret = reader.Read(pkt);
    //出错或读到末尾
    if (ret) {
      delete pkt;
      pkt = nullptr;
      break;
    }
    if (pkt->GetIndex() == videoStreamIndex) {
      videoDecoderThread->PutPacket(pkt);
    }

    if (pkt->GetIndex() == audioStreamIndex) {
      audioDecoderThread->PutPacket(pkt);
    }
    //将Packet 放入缓存
    //std::cout << "Read Packet Success" << std::endl;
    // delete pkt;
    // pkt = nullptr;
  }
  videoDecoderThread->Stop();
  audioDecoderThread->Stop();
    std::cout<<"end"<<std::endl;

  reader.Close();
}