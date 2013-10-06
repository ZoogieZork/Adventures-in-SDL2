
cmake_minimum_required(VERSION 2.8)

project(adventures-in-sdl2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

include(cotire)

if(UNIX)
	add_definitions("-std=c++0x")
endif()

add_executable(adventures-in-sdl2
	src/StdAfx.cpp
	src/Exception.h
	src/main.cpp)
set_target_properties(adventures-in-sdl2 PROPERTIES
	COTIRE_ADD_UNITY_BUILD FALSE)
set_target_properties(adventures-in-sdl2 PROPERTIES
	COTIRE_CXX_PREFIX_HEADER_INIT "src/StdAfx.h")
cotire(adventures-in-sdl2)

if(UNIX)
	include(FindPkgConfig)
	pkg_check_modules(DEPS REQUIRED sdl2 SDL2_image SDL2_ttf)

	include_directories(${DEPS_INCLUDE_DIRS})
	link_directories(${DEPS_LIBRARY_DIRS})
	target_link_libraries(adventures-in-sdl2 ${DEPS_LIBRARIES})
	target_link_libraries(adventures-in-sdl2_unity ${DEPS_LIBRARIES})
endif()

add_custom_target(run
	COMMAND adventures-in-sdl2
	DEPENDS adventures-in-sdl2
	WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})

