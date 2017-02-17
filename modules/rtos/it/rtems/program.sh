#!/bin/bash
#
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

TMPDIR=".grmon"
BAUD=460800

if [ -e $GRMON ]; then
GRMON="/opt/grmon-eval/bin/grmon"
fi

if [ -z $2 ]; then
PORT="/dev/outpost_dsu_3"
else
PORT=$2
fi

mkdir -p $TMPDIR
echo "load $1
run
exit" > $TMPDIR/program.cfg
echo $GRMON -uart $PORT -stack 0x40ffffff -baud $BAUD -c $TMPDIR/program.cfg
$GRMON -uart $PORT -stack 0x40ffffff -baud $BAUD -c $TMPDIR/program.cfg
rm -rf $TMPDIR
