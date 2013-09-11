
all: test

TERMINAL ?= /dev/cobc_console_1

PORT     ?= /dev/cobc_dsu_1
GRMON    ?= "/opt/grmon-eval-2.0.37/bin/grmon"
BAUD     ?= 460800

include ../../conf/module.default.mk

test-rtems_periodic_task:
test-%:
	@scons $(MAKEJOBS) -Q -C test/integration/$*

program-rtems_periodic_task:
program-%: test-%
	@../../tools/grmon_program.sh ./build/test/integration/$*/$* PORT=$(PORT)

terminal:
	@../../tools/terminal.sh $(TERMINAL)

grmon:
	@$(GRMON) -uart $(PORT) -stack 0x40ffffff -baud $(BAUD)

test:
	@scons build $(MAKEJOBS) -Q -C test/unit || return 1; \
	./build/test/unit/unittest --gtest_filter=$(FILTER)

clean: clean_default
	@$(RM) -r build/*
	@$(RM) -r bin/*

.PHONY: test clean
