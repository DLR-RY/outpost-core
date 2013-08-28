
all: test

include ../../conf/module.default.mk

test:
	mkdir -p build/test/unit
	cd build/test/unit; \
	cmake ../../../test/unit
	$(MAKE) -C build/test/unit --no-print-directory
	./build/test/unit/unittest

clean: clean_default
	rm -rf build/*

.PHONY: clean test
