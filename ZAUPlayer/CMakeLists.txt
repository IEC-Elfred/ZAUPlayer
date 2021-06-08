# CMakeList.txt: ZAUPlayer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

include_directories(../)
find_package(Threads)
link_directories(../3rdpart/ffmpeg/lib)

# 将源代码添加到此项目的可执行文件。
add_executable (ZAUPlayer "ZAUPlayer.cpp" )

target_link_libraries(ZAUPlayer MMThread)
target_link_libraries(ZAUPlayer MMAV)
target_link_libraries(ZAUPlayer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ZAUPlayer avcodec)
target_link_libraries(ZAUPlayer avdevice)
target_link_libraries(ZAUPlayer avformat)
target_link_libraries(ZAUPlayer avutil)
target_link_libraries(ZAUPlayer swresample)
target_link_libraries(ZAUPlayer swscale)

# TODO: 如有需要，请添加测试并安装目标。
