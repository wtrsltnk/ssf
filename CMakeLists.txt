cmake_minimum_required(VERSION 3.7)

project(ssf)

list(APPEND CMAKE_PREFIX_PATH "$ENV{userprofile}/.cpppm/install")

find_package(system.io REQUIRED)
find_package(system.net REQUIRED)

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
