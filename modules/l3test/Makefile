

all: test

MODULE=l3test

include ../../conf/module.default.mk

# Build the C++ extensions
build-lua:
	@scons -Q -C src/lua/module

# Run the unit tests
test-lua: build-lua
	@busted --lpath="lua_src/?.lua;lua_src/?/init.lua" --cpath="bin/lua/?.so" lua_spec

test-cpp:
	@scons -Q -C test/ -j3
	@../../build/$(MODULE)/test/unit/unittest

test: build-lua test-lua test-cpp

coverage:
	@scons -Q -C test/unit coverage=1 -j3
	@../../build/$(MODULE)/test/coverage/unittest

clean:
	@scons -Q -C test -c
	@scons -Q -C src/lua/module -c

.PHONY: build-lua test-lua test-cpp test clean
