cmake_minimum_required(VERSION 3.5)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_policy(SET CMP0177 NEW)
project(ChessAppBuildTest)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/libs")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


add_executable(ChessAppBuildTest ./src/main.cpp)
target_link_libraries(ChessAppBuildTest PRIVATE SFML::Graphics)