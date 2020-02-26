#!/bin/bash

echo Number of arguments passed is $#
i=1
for arg in "$@"
do
	echo arg"$i" = "$arg"
	(( i++ ))
done > cat_args_patch_2_log.txt
cat "cat_args_patch_2_log.txt"


