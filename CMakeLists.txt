cmake_minimum_required(VERSION 3.23)

project(Arduino-Library-BasicTimer LANGUAGES CXX)

add_library(arduino_BasicTimer INTERFACE)

target_include_directories(arduino_BasicTimer
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src
)