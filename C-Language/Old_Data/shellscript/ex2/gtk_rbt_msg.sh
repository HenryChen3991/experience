#!/bin/sh
#
#ifdef ODM_GEMTEK
#
#Gemtek script for sending reboot message to telnet

RESET_TIME=1
RESTORE__TO_DEFAULT_TIME=5
echo $1----$2----$3
echo "*** Reset button pressed (Pressed `expr $2 / 1000` ~ `expr $3 / 1000 - 1` sec do reset, `expr $3 / 1000` sec reset to default) ***" 