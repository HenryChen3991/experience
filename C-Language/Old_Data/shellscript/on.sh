#!/bin/bash
#Program:
#   test user whether logged in or not
#   check arguments number whether correct or not

if [ "$#" -ne 1 ]
then
    echo "Incorrect number of arguments"
    echo "Usage: ./on.sh user"
else
    user="$1"
    if who | grep "^$user " > /dev/null
    then 
        echo "$user is logged on"
    else
        echo "$user is not logged on"
    fi
fi
