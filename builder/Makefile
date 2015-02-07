
BUILDER = ~/development/library-builder/src/library-builder.py

all: generate build run

generate:
	@$(BUILDER) -r ../modules -p library.lb

build:
	@scons -j4

run:
	@./runner

clean:
	$(RM) -r ext/
	$(RM) -r src/
	$(RM) -r test/
	$(RM) -r bin/
	$(RM) -r lua/
	$(RM) runner

.PHONY : generate build run clean

