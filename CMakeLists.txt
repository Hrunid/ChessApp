cmake_minimum_required(VERSION 3.5)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_policy(SET CMP0177 NEW)
project(ChessAppBuildTest)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/libs/SFML")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW OFF  # Ustawienie na OFF, aby pobrać pełne repozytorium

)
FetchContent_MakeAvailable(SFML)

set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/libs/TGUI")
FetchContent_Declare(TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI
    GIT_TAG v1.8.0
    GIT_SHALLOW OFF

)
FetchContent_MakeAvailable(TGUI)

add_subdirectory(libs/stockfish)
set(NNUE OFF CACHE BOOL "Disable NNUE in Stockfish")

file(GLOB SRC
    src/*.cpp
)


add_executable(ChessAppBuildTest ${SRC})
target_link_libraries(ChessAppBuildTest PRIVATE SFML::Graphics)
target_link_libraries(ChessAppBuildTest PRIVATE TGUI::TGUI)
target_link_libraries(ChessAppBuildTest PRIVATE Stockfish)

target_include_directories(ChessAppBuildTest
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/libs/stockfish
)
