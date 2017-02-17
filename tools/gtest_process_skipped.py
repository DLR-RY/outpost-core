#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2016-2017, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2016-2017, Fabian Greif (DLR RY-AVS)

# Jenkins xUnit processor requires that a skipped test contains
# the <skipped/> element and not just setting status to "notrun".
# This script adds the skipped tag for those tests.

import os
import sys
import re

notrun_filter = re.compile('(\s*<testcase[^<]+status="notrun"[^<]+)/>')

with open(sys.argv[2], 'w+') as output:
	with open(sys.argv[1], 'r') as input:
		for line in input:
			
			match = notrun_filter.match(line)
			if match:
				testcase = match.group(1)
				output.write(testcase + "><skipped /></testcase>\n")
			else:
				output.write(line)
