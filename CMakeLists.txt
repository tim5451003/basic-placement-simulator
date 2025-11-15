cmake_minimum_required(VERSION 3.10)
project(BasicPlacementSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    model/placement.cc
    io/reader.cc
    cost/cost.cc
    opt/anneal.cc
    legal/legalize.cc
    detail/detail_place.cc
    viz/write_json.cc
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# Create executable
add_executable(placement_simulator ${SOURCES})

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

