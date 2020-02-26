#!/bin/sh


while [ -n "$1" ]  

do  

  case "$1" in  

  -a)  

     echo "option -a";;  

  -b)   

    value="$2"    

    echo "option -b with para $value"  

   shift;;  


 esac  

 shift  

done  

