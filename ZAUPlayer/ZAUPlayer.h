#include "../MMAV/MMAV.h"
#include "../MMQueue/MMQueue.h"
#include "../MMThread/MMThread.h"
#include "string"
#include <string>

class ZAUPlayerReaderThread;
class ZAUPlayerDecoderThread;
class ZAUPlayerController;
enum ZAUDecoderType { ZAUDECODER_TYPE_VIDEO = 0, ZAUDECODER_TYPE_AUDIO = 1 };

enum ZAUPlayerCtrStatus {
  ZAUPLAYER_CTR_STATUS_PLAYING = 0,
  ZAUPLAYER_CTR_STATUS_PAUSING = 1
};

class ZAUPlayer {
public:
  ZAUPlayer(std::string path);
  ~ZAUPlayer();
  int Open(double time = 0.0);
  int Stop();
  int Play();
  int Pause();
  int Seek(double time);

private:
  std::string path;
  ZAUPlayerReaderThread *readerThread = nullptr;
  ZAUPlayerController *playerCtl = nullptr;
};

class ZAUPlayerReaderThread : public MMThread {
public:
  ZAUPlayerReaderThread(std::string path, double seekTime,
                        ZAUPlayerController *playCtr);
  ~ZAUPlayerReaderThread();
  virtual void run();

private:
  double seekTime = 0.0;
  std::string path;
  ZAUPlayerController *playerCtr = nullptr;
};

class ZAUPlayerDecoderThread : public MMThread {
public:
  ZAUPlayerDecoderThread(ZAUPlayerController *playerCtr, ZAUDecoderType type);
  ~ZAUPlayerDecoderThread();
  virtual void run();
  int Init(MMAVStream *avstream);
  int PutPacket(MMAVPacket *pkt);
  int GetPacketQueueSize();

private:
  MMAVDecoder *decoder = nullptr;
  MMQueue<MMAVPacket> packetQueue;
  ZAUPlayerController *playerCtr = nullptr;
  ZAUDecoderType type;
};

class ZAUPlayerController : public MMThread {

public:
  ZAUPlayerController(double seekTime = 0.0);
  ~ZAUPlayerController();
  virtual void run();
  int Play();
  int Pause();
  int GetVideoQueueSize();
  int GetAudioQueueSize();
  int PushFrameToVideoQueue(MMAVFrame *frame);
  int PushFrameToAudioQueue(MMAVFrame *frame);

private:
  std::string path;
  double seekTime = 0.0;
  ZAUPlayerCtrStatus status = ZAUPLAYER_CTR_STATUS_PLAYING;
  MMQueue<MMAVFrame> videoQueue;
  MMQueue<MMAVFrame> audioQueue;
};
