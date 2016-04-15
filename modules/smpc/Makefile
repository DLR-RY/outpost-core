# Copyright (c) 2013, German Aerospace Center (DLR)
# All Rights Reserved.

MODULE=smpc

LCOV_REMOVE_PATTERN = \
	"utils/*" "time/*" "rtos/*"

all: test

include ../../conf/module.default.mk

test: test-default

coverage: coverage-default

clean: clean_default

distclean: distclean-default

.PHONY: test coverage clean
