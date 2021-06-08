//#include "../MMAV/MMAV.h"
#include "../MMAV/MMAV.h"
#include "../MMThread/MMThread.h"
#include <chrono>
#include <iostream>
#include <thread>

class MyMThread : public MMThread {
private:
  int a = 0;

public:
  virtual void run() { std::cout << "MyMThread :" << a << std::endl; }

  MyMThread(int _a) { a = _a; }
};

int main() {
  MMAVReader reader;
  int ret = reader.Open("/home/jinlin/projects/zauberflote/Reset_position.mp4");
  if (ret) {
    std::cout << "Open File Fail!!!" << std::endl;
    return -1;
  }
  int streamCount = reader.GetStreamCount();
  for (int i = 0; i < streamCount; i++) {
    MMAVStream avStream;
    reader.GetStream(&avStream, i);
    std::cout << "StreamIndex:" << avStream.streamIndex << std::endl;
    MMAVDecoder* decoder = new MMAVDecoder();
    int ret = decoder->Init(&avStream);
    if (ret) {
      std::cout << "Init decoder fail" << std::endl;
    }
  }
  while (1) {
    MMAVPacket pkt;
    
    if (reader.Read(&pkt)) {
      break;
    }
     std::cout << "Reading packets sucessful!" << std::endl;
  }
  reader.Close();
  return 0;
}
