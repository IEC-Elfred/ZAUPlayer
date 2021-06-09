#include "../MMAV/MMAV.h"
#include "../MMQueue/MMQueue.h"
#include "../MMThread/MMThread.h"
#include "string"
#include <string>

class ZAUPlayerReaderThread;
class ZAUPlayerDecoderThread;
class ZAUPlayer {
public:
  ZAUPlayer(std::string path);
  ~ZAUPlayer();
  int Open();
  int Stop();
  int Play();
  int Pause();
  int Seek(double time);

private:
  std::string path;
  ZAUPlayerReaderThread *readerThread = nullptr;
};

class ZAUPlayerReaderThread : public MMThread {
public:
  ZAUPlayerReaderThread(std::string path);
  ~ZAUPlayerReaderThread();
  virtual void run();

private:
  std::string path;
};

class ZAUPlayerDecoderThread : public MMThread {
public:
  ZAUPlayerDecoderThread();
  ~ZAUPlayerDecoderThread();
  virtual void run();
  int Init(MMAVStream *avstream);
  int PutPacket(MMAVPacket *pkt);
  int GetPacketQueueSize();

private:
  MMAVDecoder *decoder = nullptr;
  MMQueue<MMAVPacket> packetQueue;
};
