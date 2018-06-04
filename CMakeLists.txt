cmake_minimum_required(VERSION 3.5)
project(AAL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp generator.cpp generator.h Naive.cpp Naive.h BSF.cpp BSF.h IDAStar.cpp IDAStar.h Application.cpp Application.h)
add_executable(AAL ${SOURCE_FILES})
