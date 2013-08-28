
all: test

include ../../conf/module.default.mk

#test-rtems:
#	mkdir -p build/test/integration/rtems
#	cd build/test/integration/rtems; \
#	cmake -DCMAKE_TOOLCHAIN_FILE=../../../../conf/cmake/leon3-rtems.cmake ../../../../test/integration/rtems
#	$(MAKE) -C build/test/integration/rtems install --no-print-directory
#
#test-posix_timer:
#	mkdir -p build/test/integration/posix_timer
#	cd build/test/integration/posix_timer; \
#	cmake ../../../../test/integration/posix_timer
#	$(MAKE) -C build/test/integration/posix_timer --no-print-directory
#	./build/test/integration/posix_timer/test-posix_timer

test:
	@scons build $(MAKEJOBS) -Q -C test/unit || return 1; \
	./build/test/unit/unittest --gtest_filter=$(FILTER)

clean: clean_default
	@$(RM) -r build/*
	@$(RM) -r bin/*

.PHONY: test clean
