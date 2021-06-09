#include "../MMAV/MMAV.h"
#include "../MMQueue/MMQueue.h"
#include "../MMThread/MMThread.h"
#include <bits/types/FILE.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>

class MyMThread : public MMThread {
private:
  int a = 0;

public:
  virtual void run() { std::cout << "MyMThread :" << a << std::endl; }

  MyMThread(int _a) { a = _a; }
};

int main() {
  MMAVReader reader;
  MMQueue<MMAVPacket> packetQueue;
  int ret =
      reader.Open("/home/jinlin/projects/zauberflote/Tor_Animation_en.mp4");
  if (ret) {
    std::cout << "Open File Fail!!!" << std::endl;
    return -1;
  }

  int videoStreamIndex = reader.GetVideoStreamIndex();
  int audioSteamIndex = reader.GetAudioStreamIndex();
  std::printf("VideoStreamIndex: %d\n", videoStreamIndex);
  std::printf("AudioStreamIndex: %d\n", audioSteamIndex);

  std::vector<MMAVDecoder *> decoderList;
  int streamCount = reader.GetStreamCount();
  for (int i = 0; i < streamCount; i++) {
    MMAVStream avStream;
    reader.GetStream(&avStream, i);
    std::cout << "StreamIndex:" << avStream.streamIndex << std::endl;
    MMAVDecoder *decoder = new MMAVDecoder();
    int ret = decoder->Init(&avStream);
    if (ret) {
      std::cout << "Init decoder fail" << std::endl;
    }
    decoderList.push_back(decoder);
  }

   FILE *f = fopen("/home/jinlin/projects/zauberflote/demo_video.yuv", "wb");

  //读取packet
  while (1) {
    MMAVPacket *pkt = new MMAVPacket();
    if (reader.Read(pkt)) {
      break;
    }
    packetQueue.Push(pkt);
    int streamIndex = pkt->GetIndex();
    MMAVDecoder *decoder = decoderList[streamIndex];
    ret = decoder->SendPacket(pkt);
    //如果这个packet有问题就再读一个
    if (ret) {
      continue;
    }

    while (1) {
      MMAVFrame frame;
      ret = decoder->RecvFrame(&frame);
      if (ret) {
        break;
      }

      // recv Sucessful
      if (streamIndex == videoStreamIndex) {
        frame.VideoPrint();

        int width = frame.GetW();
        int height = frame.GetH();
        unsigned char *y = (unsigned char *)malloc(width * height);
        unsigned char *u = (unsigned char *)malloc(width / 2 * height / 2);
        unsigned char *v = (unsigned char *)malloc(width / 2 * height / 2);

        frame.GetY(y);
        frame.GetU(u);
        frame.GetV(v);

        fwrite(y, width * height, 1, f);
        fwrite(u, width / 2 * height / 2, 1, f);
        fwrite(v, width / 2 * height / 2, 1, f);

        free(y);
        free(u);
        free(v);

      } else if (streamIndex == audioSteamIndex) {
        frame.AudioPrint();
      }
    }
  }

  //读取decoder缓存的packet
  for (int i = 0; i < decoderList.size(); i++) {
    MMAVDecoder *decoder = decoderList[i];
    ret = decoder->SendPacket(nullptr);
    //一个packet可能有很多frame
    while (1) {
      MMAVFrame frame;
      ret = decoder->RecvFrame(&frame);
      if (ret) {
        break;
      }
      //Recv Success

    }
  }

  while (packetQueue.Size() > 0) {
    MMAVPacket *pkt = nullptr;
    packetQueue.Pop(&pkt);

    printf("Packet Size(): %d\n", packetQueue.Size());

    if (pkt != nullptr) {
      delete pkt;
    }
  }

  reader.Close();

  for (int i = 0; i < decoderList.size(); i++) {
    MMAVDecoder *decoder = decoderList[i];
    decoder->Close();
    delete decoder;
  }
  decoderList.clear();

  fclose(f);
  return 0;
}
