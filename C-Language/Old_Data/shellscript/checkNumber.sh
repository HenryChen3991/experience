#!/bin/sh

checkNumber(){
    case "$1" in
        [1-9])
	    return 1;;
        [1-9][:digit:]*)
            return 1;; 
        *)  
            return 0;; 
    esac
}




checkNumber $1
ret=$?
echo "$ret"
