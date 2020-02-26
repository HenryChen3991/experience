#!/bin/bash
mount_path="/userfs/data"
echo $mount_path


checkNumber(){
	echo "parameter = $1"
	case "$1" in
		[1-9][0-9]*)
			return 1;;
                *)
			return 0;;
	esac
}

while getopts "a:b:c:?" argv
do
	case "$argv" in
		a) 
			checkNumber $OPTARG
			ret=$?
			if [ $ret = "1" ] ;then
				echo "correct"
			else
				echo "not correct"
			fi
 
#			case "$OPTARG" in
#				[1-9][0-9]*)
#					echo "is number";;
#			esac
			var1="$OPTARG"
			echo "var1 = $var1";;
		b)  
			var2="$OPTARG"
			echo "var2 = $var2";;
		c)
			var3="$OPTARG"
			echo "var3 = $var3";;
		?)
			echo "usage:"
			echo "    -a parameter"
			echo "    -b parameter"
			echo "    -c parameter"
	esac
done

