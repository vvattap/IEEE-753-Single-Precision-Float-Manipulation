cmake_minimum_required(VERSION 3.17)
project(lab6FloatIEEE)

set(CMAKE_CXX_STANDARD 14)

add_executable(lab6FloatIEEE
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/lab6FloatIEEE
        cmake-build-debug/lab6FloatIEEE.cbp
        cmake-build-debug/Makefile
        "CIS2107 Lab 6 - Floats.docx"
        CMakeLists.txt
        floatsStuff.c
        floatsStuff.out
        floatStuff.out)
