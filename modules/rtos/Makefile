
all: test

TERMINAL ?= /dev/cobc_console_3

PORT     ?= /dev/cobc_dsu_3
GRMON    ?= "/opt/grmon-eval/bin/grmon"
BAUD     ?= 460800

VERBOSE  ?= 0

ifeq ($(VERBOSE),1)
VERBOSE_ARG="verbose=1"
else
VERBOSE_ARG=""
endif

MODULE=rtos

include ../../conf/module.default.mk

build-freertos_periodic_task:
build-hosted_float_endianness:
build-posix_periodic_task:
build-posix_timer:
build-posix_timestamp:
build-rtems:
build-rtems_float_endianness:
build-rtems_periodic_task:
build-%:
	@scons $(MAKEJOBS) -Q -C test/integration/$*

program-freertos_periodic_task:
program-rtems_float_endianness:
program-rtems_periodic_task:
program-%: test-%
	@../../tools/grmon_program.sh ./build/test/integration/$*/$* $(PORT) $(GRMON)

terminal:
	@../../tools/terminal.sh $(TERMINAL)

grmon:
	@$(GRMON) -uart $(PORT) -stack 0x40ffffff -baud $(BAUD)

test:
	@scons build $(MAKEJOBS) $(VERBOSE_ARG) -Q -C test || return 1; \
	../../build/$(MODULE)/test/unittest/runner --gtest_filter=$(FILTER)

coverage:
	@-scons build coverage=1 $(MAKEJOBS) $(VERBOSE_ARG) -Q -C test || return 1; \
	cd ../../build/$(MODULE)/test/coverage; \
	find . -name '*.gcda' | xargs -r rm; \
	./runner; \
	lcov -t '$(MODULE) library' -o coverage.info -c --base-directory ../../../../modules/$(MODULE)/src --directory src --no-external --gcov-tool `which gcov-4.7` || return 1; \
	rm -rf report; \
	genhtml -o report coverage.info

coverage-view:
	xdg-open ../../build/$(MODULE)/test/coverage/report/index.html &

clean: clean_default
	@$(RM) -r ../../build/$(MODULE)/*

distclean: distclean_default

.PHONY: test coverage coverage-view clean

