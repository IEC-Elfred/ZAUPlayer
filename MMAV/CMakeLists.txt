cmake_minimum_required(VERSION 3.8)
aux_source_directory(. DIR_LIB_SRCS)
include_directories(../3rdpart/ffmpeg/include)
add_library(
    MMAV
    MMAVReader.cpp
    MMAVPacket.cpp
)