cmake_minimum_required(VERSION 3.7)

include(cmake/get_cpm.cmake)
include(cmake/Dependencies.cmake)

project(ssf)

add_executable(ssf
    src/program.cpp
    )

target_link_libraries(ssf
    PUBLIC
        system.io
        system.net
        ws2_32
    )

target_compile_features(ssf
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    )
