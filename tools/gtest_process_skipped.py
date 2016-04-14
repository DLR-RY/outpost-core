#!/usr/bin/env python3
# 
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
