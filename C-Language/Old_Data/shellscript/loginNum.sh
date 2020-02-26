#!/bin/bash
#Program:
#    show how many user logged in  server

echo There are $(who | wc -l) users logged in server
echo "$(who)"   #use " " to reserve whitespace"

