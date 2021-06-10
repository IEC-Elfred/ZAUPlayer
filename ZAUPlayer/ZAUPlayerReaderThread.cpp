#include "ZAUPlayer.h"
#include <iostream>

ZAUPlayerReaderThread::ZAUPlayerReaderThread(std::string _path, double _seekTime, ZAUPlayerController* _playerCtr) {
  playerCtr = _playerCtr;
  path = _path;
  seekTime = _seekTime;
}

ZAUPlayerReaderThread::~ZAUPlayerReaderThread() {}

void ZAUPlayerReaderThread::run() {
  MMAVReader reader;
  int ret = reader.Open(path.c_str());
  if (ret) {
    return;
  }

  reader.Seek(seekTime);
  
  int videoStreamIndex = reader.GetVideoStreamIndex();
  int audioStreamIndex = reader.GetAudioStreamIndex();
  ZAUPlayerDecoderThread *videoDecoderThread = new ZAUPlayerDecoderThread(playerCtr,ZAUDecoderType::ZAUDECODER_TYPE_VIDEO);
  ZAUPlayerDecoderThread *audioDecoderThread = new ZAUPlayerDecoderThread(playerCtr,ZAUDecoderType::ZAUDECODER_TYPE_AUDIO);

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