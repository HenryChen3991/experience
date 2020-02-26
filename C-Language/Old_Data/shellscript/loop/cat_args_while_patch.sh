#!/bin/bash

while [ "$#" -ne 0 ]
do
	echo "$1"
	shift   #process the arguments $1 $1<-$2 ...
done
