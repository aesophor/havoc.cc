cmake_minimum_required(VERSION 3.9)
project(havoc VERSION 1.0.5)

set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")

find_package(SDL2 REQUIRED HINTS /usr/local)
message(STATUS "SDL2_INCLUDE_DIRS=${SDL2_INCLUDE_DIRS}")

include_directories("src")
include_directories("thirdparty")
include_directories("thirdparty/imgui")
include_directories("/usr/local/include")
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})

add_library(
	${PROJECT_NAME} SHARED

	src/Core/Dylib.cc
	src/Core/Hooks.cc
	src/Core/Interfaces.cc
	src/Core/NetVars.cc
	src/Hacks/Skins.cc
	src/Hacks/Aimbot.cc
  src/Hacks/AutoWall.cc
  src/Hacks/Chams.cc
	src/Hacks/Bhop.cc
	src/Hacks/Glow.cc
	src/GUI/SDL.cc
	src/GUI/GUI.cc
	src/SDK/CEntity.cc
	src/Main.cc

	thirdparty/imgui/imgui.cpp
	thirdparty/imgui/imgui_draw.cpp
	thirdparty/imgui/imgui_tables.cpp
	thirdparty/imgui/imgui_widgets.cpp
	thirdparty/imgui/backends/imgui_impl_sdl.cpp
	thirdparty/imgui/backends/imgui_impl_opengl2.cpp
)

target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
