
all: test

test:
	mkdir -p build/test/unit
	cd build/test/unit; \
	cmake ../../../test/unit
	$(MAKE) -C build/test/unit --no-print-directory
	./build/test/unit/unittest

coverage:
	mkdir -p build/test/coverage
	cd build/test/coverage; \
	cmake -DCMAKE_TOOLCHAIN_FILE=../../../conf/cmake/hosted-gcov.cmake ../../../test/unit; \
	$(MAKE) || return 1; \
	find . -name '*.gcda' | xargs -r rm; \
	./unittest; \
	lcov -t 'SMPC library' -o coverage.info -c -b ../../../src -d build-smpc --no-external --gcov-tool gcov-4.7 || return 1; \
	rm -rf report; \
	genhtml -o report coverage.info

coverage-view:
	xdg-open build/test/coverage/report/index.html &

doxygen:
	cd doc/doxygen; \
	doxygen

doc: doxygen

clean:
	rm -rf build/*

.PHONY: clean doxygen test

