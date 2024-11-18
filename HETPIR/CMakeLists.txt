# Copyright (C) 2020 Intel Corporation
# SPDX-License-Identifier: Apache-2.0

project(HEPIR_example LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})
# Example using HEXL library
find_package(HEXL 1.2.5
    HINTS ${HEXL_HINT_DIR}
    REQUIRED)

# Ensure HEXL target exists
if (NOT TARGET HEXL::hexl)
    message(FATAL_ERROR "TARGET HEXL::hexl not found")
endif()
# Source files
set(SOURCES
    main.cpp
    fheoperations.cpp
    preprocess.cpp
    params.cpp
    client.cpp
    server.cpp
)
set(CMAKE_BUILD_TYPE Debug)

# Add the executable for HEPIR.cpp
add_executable(HEPIR_example ${SOURCES})

# Link HEXL library to the executable
target_link_libraries(HEPIR_example PRIVATE HEXL::hexl)


