#include "ZAUPlayer.h"
#include <chrono>
#include <iostream>
#include <thread>
ZAUPlayerController::ZAUPlayerController(double _seekTime) {
  seekTime = _seekTime;
}

ZAUPlayerController::~ZAUPlayerController() {}

void ZAUPlayerController::run() {
  ZAUPlayerReaderThread readerThread(
      "/home/jinlin/projects/zauberflote/Tor_Animation_en.mp4", seekTime, this);
  readerThread.Start();
  //获取线程启动时候的时间 start_time
  long long startTime = MMAVTime::GetTime();
  MMAVFrame *videoFrame = nullptr;
  MMAVFrame *audioFrame = nullptr;
  long long sleepCountTime = 0;

  while (!stopFlag) {
    //挂一段时间，防止来回循环
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    long long sleepTimeStart = MMAVTime::GetTime();
    while (status == ZAUPLAYER_CTR_STATUS_PAUSING) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    long long sleepTimeEnd = MMAVTime::GetTime();
    long long sleepDTime = sleepTimeEnd - sleepTimeStart;
    //暂停了几次
    sleepCountTime += sleepDTime;

    //获取当前的时间now_time
    long long nowTime = MMAVTime::GetTime();
    //获取当前时间和开始时间的差值d_time
    // dTime单位是毫秒
    long long dTime = nowTime - startTime;
    dTime = dTime - sleepCountTime;

    dTime = dTime + (long long)(seekTime * 1000);
    //从视频缓存队列中，获取一个视频pts
    if (videoFrame == nullptr) {
      //尝试取出一帧
      videoQueue.Pop(&videoFrame);
    }

    if (videoFrame != nullptr) {
      if (videoFrame->GetPts() < (long long)(seekTime * 1000)) {
        delete videoFrame;
        videoFrame = nullptr;
      }
    }

    if (videoFrame != nullptr) {

      //如果pts<d_time
      //这时候这个帧应该立即被播放
      if (videoFrame->GetPts() <= dTime) {
        std::cout << "Video Frame :" << videoFrame->GetPts() << std::endl;
        delete videoFrame;
        videoFrame = nullptr;
      } else {
        //否则，这帧视是频还不到播放时候，程序进行自旋，或者是去处理音频
      }
    }

    //从音频缓存队列中，获取一个frame视频pts
    if (audioFrame == nullptr) {
      audioQueue.Pop(&audioFrame);
    }

    if (audioFrame != nullptr) {
      if (audioFrame->GetPts() < (long long)(seekTime * 1000)) {
        delete audioFrame;
        audioFrame = nullptr;
      }
    }

    if (audioFrame != nullptr) {
      if (audioFrame->GetPts() <= dTime) {
        std::cout << "Audio Frame" << audioFrame->GetPts() << std::endl;
        delete audioFrame;
        audioFrame = nullptr;
      } else {
      }
    }
  }
}

int ZAUPlayerController::Play() {
  status = ZAUPLAYER_CTR_STATUS_PLAYING;
  return 0;
}
int ZAUPlayerController::Pause() {
  status = ZAUPLAYER_CTR_STATUS_PAUSING;
  return 0;
}

int ZAUPlayerController::GetVideoQueueSize() { return videoQueue.Size(); }
int ZAUPlayerController::GetAudioQueueSize() { return audioQueue.Size(); }
int ZAUPlayerController::PushFrameToVideoQueue(MMAVFrame *frame) {
  return videoQueue.Push(frame);
}
int ZAUPlayerController::PushFrameToAudioQueue(MMAVFrame *frame) {
  return audioQueue.Push(frame);
}
