cmake_minimum_required(VERSION 3.23)

project(Arduino-BasicTimer-Tests LANGUAGES CXX)

##############################
# Testing depends on https://github.com/rtnate/arduino-MockAndTesting
# in order to keep the library repo clean, this library is kept 
# out of version control.  When building tests with CMake set 
# cache variable TEST_MOCK_DIRECTORY to the location of this library
##############################
set(TEST_LIB_DIR ${TEST_MOCK_DIRECTORY})

# Include RT ElecTRonix Arduino testing helper script
include(${TEST_LIB_DIR}/rt_arduino_test.cmake)

####################################
# Set name of library test target here
####################################
set(TEST_LIB_NAME arduino_BasicTimer)



enable_testing()

rt_create_test_suite(${TEST_LIB_NAME}-tests
    ${CMAKE_CURRENT_LIST_DIR}/tests/VerifyGTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tests/UnitTests/BasicTimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tests/UnitTests/BasicTimerCallbacks.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tests/UnitTests/StaticTimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tests/UnitTests/StaticTimerCallbacks.cpp
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../ lib${TEST_LIB_NAME})
add_subdirectory(${TEST_LIB_DIR}/MockArduino libMockArduino)
target_link_libraries(${TEST_LIB_NAME} INTERFACE arduino_MockCore)
target_link_libraries(${TEST_LIB_NAME}-tests PUBLIC ${TEST_LIB_NAME})

include(GoogleTest)
gtest_discover_tests(${TEST_LIB_NAME}-tests)



