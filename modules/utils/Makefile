
all: test

MODULE=utils

include ../../conf/module.default.mk

test:
	@scons build $(MAKEJOBS) -Q -C test || return 1; \
	../../build/$(MODULE)/test/unittest/runner --gtest_filter=$(FILTER)

coverage:
	@scons build coverage=1 $(MAKEJOBS) -Q -C test || return 1; \
	find ../../build/$(MODULE)/test/coverage -name "*.gcda" -delete; \
	../../build/$(MODULE)/test/coverage/runner; \
	cd ../../build/$(MODULE)/test/coverage; \
	lcov -t '$(MODULE) library' -o coverage.info -c --base-directory ../../../../modules/$(MODULE)/test --directory . --gcov-tool `which gcov` || return 1; \
	lcov -r coverage.info "/usr*" "test/*" "time/*" "utils/ext/*" "tools/*" "rtos/*" "default/*" -o coverage.info; \
	rm -rf report; \
	genhtml -o report coverage.info

coverage-view:
	xdg-open ../../build/$(MODULE)/test/coverage/report/index.html &

clean: clean_default
	@$(RM) -r ../../build/$(MODULE)/*

distclean: distclean_default

.PHONY: test coverage coverage-view clean

