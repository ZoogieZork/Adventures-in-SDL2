
cmake_minimum_required(VERSION 2.8)

project(adventures-in-sdl2)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

include(CXX11)
include(cotire)

# Enable C++11 support if available.
# Even if we don't detect it, try to build anyway and hope for the best :)
check_for_cxx11_compiler(CXX11_COMPILER)
if(CXX11_COMPILER)
	enable_cxx11()
endif()

# Crank up the warning level.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

if(UNIX)
	include(FindPkgConfig)
	pkg_check_modules(DEPS REQUIRED sdl2 SDL2_image SDL2_mixer SDL2_ttf)
	include_directories(${DEPS_INCLUDE_DIRS})
	link_directories(${DEPS_LIBRARY_DIRS})
elseif(WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/include)
	link_directories(${CMAKE_SOURCE_DIR}/lib)
endif()

set(AISDL_SRCS
	src/StdAfx.cpp
	src/StdAfx.h
	src/App.cpp
	src/App.h
	src/ClockDecor.cpp
	src/ClockDecor.h
	src/Config.cpp
	src/Config.h
	src/Director.h
	src/Display.cpp
	src/Display.h
	src/Exception.h
	src/FinalScene.cpp
	src/FinalScene.h
	src/FmtTextDecor.cpp
	src/FmtTextDecor.h
	src/InitScene.cpp
	src/InitScene.h
	src/IntroScene.cpp
	src/IntroScene.h
	src/Layer.cpp
	src/Layer.h
	src/Level.cpp
	src/Level.h
	src/LevelDecor.cpp
	src/LevelDecor.h
	src/MainLoopScene.cpp
	src/MainLoopScene.h
	src/PagedTextDecor.cpp
	src/PagedTextDecor.h
	src/Player.cpp
	src/Player.h
	src/PlayerDecor.cpp
	src/PlayerDecor.h
	src/PreloadScene.cpp
	src/PreloadScene.h
	src/Res.cpp
	src/Res.h
	src/ResStr.cpp
	src/ResStr.h
	src/Scene.cpp
	src/Scene.h
	src/SideScrollScene.cpp
	src/SideScrollScene.h
	src/SpriteMap.cpp
	src/SpriteMap.h
	src/TextInputScene.cpp
	src/TextInputScene.h
	src/Ttf.cpp
	src/Ttf.h
	src/main.cpp
	src/adventures-in-sdl2.rc
	src/resource.h)

add_executable(adventures-in-sdl2 WIN32 ${AISDL_SRCS})
source_group("src" FILES ${AISDL_SRCS})
set_target_properties(adventures-in-sdl2 PROPERTIES
	COTIRE_ADD_UNITY_BUILD FALSE)
set_target_properties(adventures-in-sdl2 PROPERTIES
	COTIRE_CXX_PREFIX_HEADER_INIT "src/StdAfx.h")
cotire(adventures-in-sdl2)

if(UNIX)
	target_link_libraries(adventures-in-sdl2 ${DEPS_LIBRARIES})
elseif(WIN32)
	target_link_libraries(adventures-in-sdl2 SDL2 SDL2main SDL2_image SDL2_mixer SDL2_ttf)
	file(GLOB DLLS RELATIVE ${CMAKE_SOURCE_DIR} lib/*.dll)
	file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(UNIX)
	add_custom_target(run
		COMMAND adventures-in-sdl2
		DEPENDS adventures-in-sdl2
		WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})
endif()
