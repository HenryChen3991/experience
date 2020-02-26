#!/bin/bash
#Program:
#    test cmd test
echo "cmd test script"
echo "compare string"

str1="hello"
if test "$str1" = "hello"
then
    echo "correct"
fi

str2=""
if test "$str2" = "hello"
then
    echo "correct"
else
    echo "not correct"
fi

