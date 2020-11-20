#!/bin/sh

echo -e "aa.sh"
A=2
if [ $A -ne 1 ]
then
	return 2
fi
echo -e "bb.sh"
return 1
