
cmake_minimum_required(VERSION 2.8)

project(adventures-in-sdl2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

include(cotire)

if(UNIX)
	add_definitions("-std=c++0x")
endif()

if(UNIX)
	include(FindPkgConfig)
	pkg_check_modules(DEPS REQUIRED sdl2 SDL2_image SDL2_ttf)
	include_directories(${DEPS_INCLUDE_DIRS})
	link_directories(${DEPS_LIBRARY_DIRS})
elseif(WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/include)
	link_directories(${CMAKE_SOURCE_DIR}/lib)
endif()

add_executable(adventures-in-sdl2 WIN32
	src/StdAfx.cpp
	src/StdAfx.h
	src/App.cpp
	src/App.h
	src/Director.h
	src/Display.cpp
	src/Display.h
	src/Exception.h
	src/FinalScene.cpp
	src/FinalScene.h
	src/PreloadScene.cpp
	src/PreloadScene.h
	src/Scene.cpp
	src/Scene.h
	src/main.cpp)
set_target_properties(adventures-in-sdl2 PROPERTIES
	COTIRE_ADD_UNITY_BUILD FALSE)
set_target_properties(adventures-in-sdl2 PROPERTIES
	COTIRE_CXX_PREFIX_HEADER_INIT "src/StdAfx.h")
cotire(adventures-in-sdl2)

if(UNIX)
	target_link_libraries(adventures-in-sdl2 ${DEPS_LIBRARIES})
elseif(WIN32)
	target_link_libraries(adventures-in-sdl2 SDL2 SDL2main SDL2_image SDL2_ttf)
	file(GLOB DLLS RELATIVE ${CMAKE_SOURCE_DIR} lib/*.dll)
	file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(UNIX)
	add_custom_target(run
		COMMAND adventures-in-sdl2
		DEPENDS adventures-in-sdl2
		WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
endif()
