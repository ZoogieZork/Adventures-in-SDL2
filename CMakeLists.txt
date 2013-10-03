
cmake_minimum_required(VERSION 2.8)

project(adventures-in-sdl2)

if(UNIX)
	add_definitions("-std=c++0x")
endif()

add_executable(adventures-in-sdl2 src/main.cpp)

include(FindPkgConfig)
pkg_search_module(DEPS REQUIRED sdl2 SDL2_image SDL2_ttf)

include_directories(${DEPS_INCLUDE_DIRS})
target_link_libraries(adventures-in-sdl2 ${DEPS_LIBRARIES})

