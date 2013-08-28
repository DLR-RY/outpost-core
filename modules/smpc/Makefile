
all: test

include ../../conf/module.default.mk

test:
	@scons build $(MAKEJOBS) -Q -C test/unit || return 1; \
	./build/test/unit/unittest --gtest_filter=$(FILTER)

coverage:
	scons build coverage=1 $(MAKEJOBS) -Q -C test/unit || return 1; \
	cd build/test/coverage; \
	find . -name '*.gcda' | xargs -r rm; \
	./unittest; \
	lcov -t 'SMPC library' -o coverage.info -c -b ../../../src -d src --no-external --gcov-tool `which gcov-4.7` || return 1; \
	rm -rf report; \
	genhtml -o report coverage.info

coverage-view:
	xdg-open build/test/coverage/report/index.html &

clean: clean_default
	@$(RM) -r build/*
	@$(RM) -r bin/*

.PHONY: test coverage coverage-view clean

