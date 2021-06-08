cmake_minimum_required(VERSION 3.8)
aux_source_directory(. DIR_LIB_SRCS)
add_library(
    MThread
    ${DIR_LIB_SRCS}
)
