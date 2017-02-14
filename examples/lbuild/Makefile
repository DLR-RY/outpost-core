
BUILDER = ~/development/library-builder/scripts/lbuild

all: generate build run

generate:
	@$(BUILDER) -r ../../modules/outpost.lb -c library.lb

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

