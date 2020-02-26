#!/bin/bash
#my copy shell script

#check arguments
if [ "$#" -ne 2 ]
then
	echo "Usage: mycp from to"
	exit 1
fi

from="$1"
to="$2"

#check target file whether is dir
if [ -d "$to" ]
then
	to="$to/$(basename $from)"
fi
echo "to = $to"

#check whether target file is existed
if [ -e "$to" ]
then
	echo "$to already exists; overwrite (yes/no)? \c"    #  "\c" avoid echo auto \r\n
	read answer
	
	if [ "$answer" != yes ]
	then
		echo "Copy not performed"
		exit 0
	fi
fi

#if target file dosen't exist or user input yes
cp "$from" "$to"
