cmake_minimum_required(VERSION 3.10)

project(Mafia_Bar_SDK)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MBXMLWRITER  STATIC Source/MBXMLWRITER.cpp)

include_directories("Include")