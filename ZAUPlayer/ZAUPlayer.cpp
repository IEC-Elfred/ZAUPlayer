#include "ZAUPlayer.h"

ZAUPlayer::ZAUPlayer(std::string _path) { path = _path; }

ZAUPlayer::~ZAUPlayer() {}
int ZAUPlayer::Open() {
  if (readerThread == nullptr) {
    readerThread = new ZAUPlayerReaderThread(path);
    readerThread->Start();
    return 0;
  }
  return -1;
}
int ZAUPlayer::Stop() {
  if (readerThread != nullptr) {
    readerThread->Stop();
    delete readerThread;
    readerThread = nullptr;
  }
  return 0;
}
int ZAUPlayer::Play() { return 0; }
int ZAUPlayer::Pause() { return 0; }
int ZAUPlayer::Seek(double time) { return 0; }