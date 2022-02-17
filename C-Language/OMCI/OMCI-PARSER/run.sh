#!/bin/sh

make clean

make

MODE=""

if [ ! -f "$1" ];then
    echo -e "$1 not exist"
    exit 1;
fi

if [ "$2" == "-f" ];
then
    if [ "$3" == "" ]; then
        echo -e "args not valid"
        exit 1;
    fi
    MODE="Filter"
elif [ "$2" == "-MEReplay" ]
then
    MODE="MEreplay"
else
    MODE="Normal"
fi

if [ "$MODE" == "Filter" ];
then
    ./omciparser $1 -f $3
elif [ "$MODE" == "MEreplay" ];
then
    ./omciparser $1 -MEReplay
    echo -e "\nOutput Result:"
    cat $1-replay
elif [ "$MODE" == "Normal" ];
then
    ./omciparser $1
fi
