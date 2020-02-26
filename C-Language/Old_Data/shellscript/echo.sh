#!/bin/sh

val=$(cat a.txt)
echo "val = $val"
if [ "$val" == "" ]; then
	echo "a"
else
	echo "b"
fi
