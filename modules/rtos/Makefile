
all: test

test:
	mkdir -p build/test/unit
	cd build/test/unit; \
	cmake ../../../test/unit
	$(MAKE) -C build/test/unit --no-print-directory
	./build/test/unit/unittest

test-rtems:
	mkdir -p build/test/integration/rtems
	cd build/test/integration/rtems; \
	cmake -DCMAKE_TOOLCHAIN_FILE=../../../../conf/cmake/leon3-rtems.cmake ../../../../test/integration/rtems
	$(MAKE) -C build/test/integration/rtems install --no-print-directory

test-posix_timer:
	mkdir -p build/test/integration/posix_timer
	cd build/test/integration/posix_timer; \
	cmake ../../../../test/integration/posix_timer
	$(MAKE) -C build/test/integration/posix_timer --no-print-directory
	./build/test/integration/posix_timer/test-posix_timer

doxygen:
	cd doc/doxygen; \
	doxygen

doc: doxygen

clean:
	rm -rf build/*
	rm -rf bin/*

.PHONY: clean doxygen test
