#include "ZAUPlayer.h"

ZAUPlayer::ZAUPlayer(std::string _path) { path = _path; }

ZAUPlayer::~ZAUPlayer() {}
int ZAUPlayer::Open(double time ) {

  if (playerCtl == nullptr) {
    playerCtl = new ZAUPlayerController(time);
    playerCtl->Start();
    return 0;
  }
  return -1;
}
int ZAUPlayer::Stop() {
  if (playerCtl != nullptr) {
    playerCtl->Stop();
    playerCtl = nullptr;
  }
  return 0;
}
int ZAUPlayer::Play() {
  if (playerCtl != nullptr) {
    playerCtl->Play();
  }

  return 0;
}
int ZAUPlayer::Pause() {
  if (playerCtl != nullptr) {
    playerCtl->Pause();
  }
  return 0;
}
int ZAUPlayer::Seek(double time) {
  Stop();
  Open(time);
  return 0;
}