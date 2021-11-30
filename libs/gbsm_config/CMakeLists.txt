cmake_minimum_required(VERSION 3.5)

project(gbsm_config C)

file(GLOB SOURCES "src/*.c")
add_library(gbsm_config STATIC ${SOURCES})

target_include_directories(gbsm_config PUBLIC inc ${gbsm_config_BINARY_DIR})