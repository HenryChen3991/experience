#!/bin/sh

Delay=0
Count=10000

echo "d = $Delay"
echo "c = $Count"


if [ $Count -gt  $Delay ]; then
	echo "a"
else
	echo "b"
fi

sh date.sh

sleep 3
echo "finish"
