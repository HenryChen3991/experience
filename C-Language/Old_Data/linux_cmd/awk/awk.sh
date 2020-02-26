#!/bin/sh

usage(){
	echo usage:
	echo     sh awk.sh -o [option]
	exit 1
}

Option=0

while [ -n "$1" ]
do
    case "$1" in
        -o)
            if [ -n "$2" ] ;then
               Option="$2"
               shift
            fi
	    ;;
         *)
            usage
	    ;;
    esac
    shift
done

case "$Option" in
    1)
        echo "cmd = awk '{print \$1}' file1"
        echo result:
        awk '{print $1}' file1
	;;
esac
