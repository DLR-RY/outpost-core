#!/bin/bash

TMPDIR=".grmon"
BAUD=460800

if [ -e $2 ]; then
PORT=$2
else
PORT="/dev/cobc_dsu_1"
fi

if [ -e $3 ]; then
GRMON=$3
else
GRMON="/opt/grmon-eval/bin/grmon"
fi

mkdir -p $TMPDIR
echo "load $1
run
exit" > $TMPDIR/program.cfg
$GRMON -uart $PORT -stack 0x40ffffff -baud $BAUD -c $TMPDIR/program.cfg
rm -rf $TMPDIR

