cmake_minimum_required(VERSION 3.7)

project(ssf)

list(APPEND CMAKE_PREFIX_PATH "$ENV{userprofile}/.cpppm/install")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_PREFIX_PATH}/cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)
find_package(system.io REQUIRED)
find_package(system.net REQUIRED)

add_executable(ssf
    src/program.cpp
    )

target_link_libraries(ssf
    PUBLIC
        fmt::fmt
        system.io
        system.net
        ws2_32
    )

target_compile_features(ssf
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    )
