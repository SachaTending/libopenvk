cmake_minimum_required(VERSION 3.15)
project(libopenvk VERSION 1.0.0 LANGUAGES C)

add_library(openvk)

target_sources(openvk
    PRIVATE
        src/api.c
        src/classes/ovk.c
)

find_package(CURL)
find_package(jansson)

target_include_directories(openvk
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(openvk PRIVATE curl)
target_link_libraries(openvk PUBLIC jansson)