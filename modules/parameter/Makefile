# Copyright (c) 2013-2017, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2013-2017, Fabian Greif (DLR RY-AVS)

MODULE=parameter

LCOV_REMOVE_PATTERN = \
	"utils/*" "time/*" "rtos/*"

all: test

include ../module.default.mk

test: test-default

test-verbose: test-verbose-default

coverage: coverage-default

clean: clean-default

distclean: distclean-default

.PHONY: test coverage clean
