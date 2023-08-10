cmake_minimum_required(VERSION 3.26)
project(opentrs)

set(CMAKE_CXX_STANDARD 17)

add_executable(opentrs main.cpp
        inc/Trace.h
        src/Trace.cpp
        inc/Trace.h
        src/TraceTools.cpp
        inc/TraceTools.h
        src/BaseTools.cpp
        inc/BaseTools.h)
