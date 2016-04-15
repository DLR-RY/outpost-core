# Copyright (c) 2013, German Aerospace Center (DLR)
# All Rights Reserved.

MODULE=l3test

all: test

include ../../conf/module.default.mk

# Build the C++ extensions
build-lua:
	@scons -Q -C lua/modules/

# Run the unit tests
test-lua: build-lua
	@busted --lpath="lua/src/?.lua;lua/src/?/init.lua" --cpath="bin/lua/?.so" lua/spec

test: build-lua test-lua test-default

coverage: coverage-default

clean: clean-default
	@scons -s -C test -c
	@scons -s -C lua/modules -c

.PHONY: build-lua test-lua test-cpp test clean
