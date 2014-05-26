
all: test

MODULE=utils

include ../../conf/module.default.mk

test:
	@scons build $(MAKEJOBS) -Q -C test/unit || return 1; \
	../../build/$(MODULE)/test/unit/unittest --gtest_filter=$(FILTER)

coverage:
	@scons build coverage=1 $(MAKEJOBS) -Q -C test/unit || return 1; \
	cd ../../build/$(MODULE)/test/coverage; \
	find . -name '*.gcda' | xargs -r rm; \
	./unittest; \
	lcov -t '$(MODULE) library' -o coverage.info -c --base-directory ../../../../modules/$(MODULE)/src --directory src --no-external --gcov-tool `which gcov-4.7` || return 1; \
	rm -rf report; \
	genhtml -o report coverage.info

coverage-view:
	xdg-open ../../build/$(MODULE)/test/coverage/report/index.html &

clean: clean_default
	@$(RM) -r ../../build/$(MODULE)/*

distclean: distclean_default

.PHONY: test coverage coverage-view clean

