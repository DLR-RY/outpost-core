# Copyright (c) 2015-2017, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2015-2017, Fabian Greif (DLR RY-AVS)

BOARD=5

build:
	@scons install

program: build
	@./program.sh bin/rtems.elf /dev/outpost_dsu_$(BOARD)

terminal-console:
	@picocom -b38400 /dev/outpost_console_$(BOARD)

terminal-link:
	@picocom -b38400 /dev/outpost_link_$(BOARD)

