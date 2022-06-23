CXX = clang++
CXXFLAGS = -arch x86_64 \
           -std=c++20 \
           -g \
           -dynamiclib \
           -framework Carbon \
           -Isrc \
           -I/opt/homebrew/include \

BUILD_DIR = build
SRC_DIR = src

PWD = $(shell pwd)
SRC = $(shell find src -type f -name '*.cc')
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
