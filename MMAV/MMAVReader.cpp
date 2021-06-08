#include "MMAV.h"
extern "C" {
#include <libavformat/avformat.h>
}

MMAVReader::MMAVReader() { formatCtx = avformat_alloc_context(); }
MMAVReader::~MMAVReader() {
  if (formatCtx != nullptr) {
    avformat_free_context(formatCtx);
    formatCtx = nullptr;
  }
}

int MMAVReader::Open(const char *path) {
  if (formatCtx == nullptr) {
    return -1;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
  }
  int ret = avformat_open_input(&formatCtx, path, nullptr, nullptr);
  if (!ret) {
    avformat_find_stream_info(formatCtx, nullptr);
  }
  return ret;
}
int MMAVReader::Close() {
  if (formatCtx == nullptr) {
    return -1;
  }
  avformat_close_input(&formatCtx);
  return 0;
}
int MMAVReader::Read(MMAVPacket *packet) {

  if (formatCtx == nullptr) {
    return -1;
  }
  int ret = av_read_frame(formatCtx, packet->pkt);
  return ret;
}