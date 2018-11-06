#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2018, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2018, Fabian Greif (DLR RY-AVS)

LBUILD=python3 ~/development/modm/lbuild/

show-modules:
	$(LBUILD) -r ../../outpost.lb discover

dependencies:
	$(LBUILD) -r ../../outpost.lb dependencies > dep.dot
	dot -Grankdir=BT -Tpng dep.dot > dep.png
	xdg-open dep.png

build-all:
	#$(RM) -r source
	$(LBUILD) -r ../../outpost.lb --path source build --module "outpost:*"
	scons -j3 -C source

clean:
	$(RM) dep.dot
	$(RM) dep.png

.PHONY : show-modules dependencies clean
