CXX = clang++
CXXFLAGS = -arch x86_64 \
           -std=c++20 \
           -g \
           -dynamiclib \
           -framework OpenGL \
           `sdl2-config --cflags` \
           -Isrc \
           -Ithirdparty \
           -Ithirdparty/imgui \
           -I/usr/local/include

BUILD_DIR = build
SRC_DIR = src
IMGUI_DIR = thirdparty/imgui

PWD  = $(shell pwd)
SRC  = $(shell find src -type f -name '*.cc')
SRC += $(IMGUI_DIR)/imgui.cpp
SRC += $(IMGUI_DIR)/imgui_draw.cpp
SRC += $(IMGUI_DIR)/imgui_tables.cpp
SRC += $(IMGUI_DIR)/imgui_widgets.cpp
SRC += $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp
SRC += $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp

BIN = libHavoc.dylib
PROC = csgo_osx64

all:
	$(info $(SRC))
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BUILD_DIR)/$(BIN)

clean:
	rm -rf $(BUILD_DIR)

run:
	sudo lldb --batch \
		-o "attach --name $(PROC)" \
		-o "expr void* \$$library = (void*)dlopen(\"$(PWD)/$(BUILD_DIR)/$(BIN)\", 10)" \
		-o "detach" \
		-o "quit"

stop:
	sudo lldb --batch \
		-o "attach --name $(PROC)" \
		-o "expr void* \$$library = (void*)dlopen(\"$(PWD)/$(BUILD_DIR)/$(BIN)\", 18)" \
		-o "p (int)dlclose(\$$library)" \
		-o "p (int)dlclose(\$$library)" \
		-o "detach" \
		-o "quit"
