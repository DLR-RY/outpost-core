

all: test

MODULE=l3test

include ../../conf/module.default.mk

# Build the C++ extensions
build-lua:
	@scons -Q -C lua/modules/

# Run the unit tests
test-lua: build-lua
	@busted --lpath="lua/src/?.lua;lua/src/?/init.lua" --cpath="bin/lua/?.so" lua/spec

test-cpp:
	@scons -Q -C test/ -j3
	@../../build/$(MODULE)/test/unittest/runner

test: build-lua test-lua test-cpp

coverage:
	@scons -Q -C test/unit coverage=1 -j3
	@../../build/$(MODULE)/test/coverage/runner

clean:
	@scons -s -C test -c
	@scons -s -C lua/modules -c

.PHONY: build-lua test-lua test-cpp test clean
