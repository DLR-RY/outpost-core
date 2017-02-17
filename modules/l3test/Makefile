# Copyright (c) 2014-2017, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2014-2017, Fabian Greif (DLR RY-AVS)

MODULE=l3test

all: test

include ../module.default.mk

# Build the C++ extensions
build-lua:
	@scons -Q -C lua/modules/

# Run the unit tests
test-lua: build-lua
	@busted --lpath="lua/src/?.lua;lua/src/?/init.lua" --cpath="../../bin/lua/?.so" lua/spec

test: build-lua test-lua test-default

coverage: coverage-default

clean: clean-default
	@scons -s -C test -c
	@scons -s -C lua/modules -c

.PHONY: build-lua test-lua test-cpp test clean
