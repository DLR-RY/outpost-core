
all: test

MODULE=hal

include ../../conf/module.default.mk

test:
	
coverage:

coverage-view:
	xdg-open ../../build/$(MODULE)/test/coverage/report/index.html &

clean: clean_default
	@$(RM) -r ../../build/$(MODULE)/*

distclean: distclean_default

.PHONY: test coverage coverage-view clean
