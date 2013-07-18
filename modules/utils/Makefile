
all: test

test:
	mkdir -p build/test/unit
	cd build/test/unit; \
	cmake ../../../test/unit
	$(MAKE) -C build/test/unit --no-print-directory
	./build/test/unit/unittest

doxygen:
	cd doc/doxygen; \
	doxygen

doc: doxygen

clean:
	rm -rf build/*

.PHONY: clean doxygen test
