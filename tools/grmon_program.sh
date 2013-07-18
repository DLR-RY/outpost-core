#!/bin/bash

TMPDIR=".grmon"
#GRMON="grmon-eval"
#GRMON="grmon-2.0.32-eval"
#GRMON="grmon-2.0.29b-eval"
GRMON="/opt/grmon-eval-2.0.37/bin/grmon"
#GRMON="/opt/grmon-pro-2.0.30/bin/grmon"
BAUD=460800

if [ -e $2 ]; then
PORT=$2
else
#PORT="/dev/ttyUSB0"
PORT="/dev/cobc_dsu_1"
fi

mkdir -p $TMPDIR
echo "load $1
run
exit" > $TMPDIR/program.cfg
$GRMON -uart $PORT -stack 0x40ffffff -baud $BAUD -c $TMPDIR/program.cfg
rm -rf $TMPDIR

