
all: build test

# Build the C++ extensions
build:
	@scons

# Run the unit tests
test: build
	@busted --cpath=../bin/module/?.so spec

clean:
	@scons -c

