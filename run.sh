#!/usr/bin/env sh

cwd=`pwd`

sudo lldb --batch \
	-o "attach --name csgo_osx64" \
	-o "expr void* \$library = (void*)dlopen(\"$cwd/build/libHavoc.dylib\", 10)" \
	-o "detach" \
	-o "quit"
