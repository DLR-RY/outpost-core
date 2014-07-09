
# Build the C++ extensions
build-lua:
	@scons -Q -C src/lua/module

# Run the unit tests
test-lua: build-lua
	@busted --lpath="lua_src/?.lua;lua_src/?/init.lua" --cpath="bin/lua/?.so" test/spec

test-cpp:
	@scons -Q -C test/unit -j3
	@../../build/l3test/test/unit/unittest

test: test-cpp test-lua

coverage:
	@scons -Q -C test/unit coverage=1 -j3
	@../../build/l3test/test/coverage/unittest

clean:
	@scons -Q -C test/unit -c
	@scons -Q -C src/lua/module -c

.PHONY: test-lua test-cpp test clean
