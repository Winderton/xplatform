cmake_minimum_required(VERSION 3.12)

set(CXX_STANDARD 17)

project(serialization)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

add_executable(app ${all_SRCS})