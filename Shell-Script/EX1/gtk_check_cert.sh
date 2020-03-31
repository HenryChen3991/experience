#!/bin/sh

filelist=`ls ./600-cert/*.prv`

rm rsakey.pem

for file in $filelist
do
	fp=`ls $file | cut -c 1-11 --complement`
#	echo "start to find "$fp
	if grep -q $fp log.txt; then
  		echo find > /dev/null
	else
#		echo $fp
		openssl rsa -inform der -in $file -out rsakey.pem
#		openssl  asn1parse -i -in rsakey.pem

		len=`openssl asn1parse -i -in rsakey.pem |grep "273:d=1" | awk -F " " '{print $4}'`	
		if [ $len -ne 256 ]; then
			echo $fp >> ./fail_key.txt
		fi
		rm rsakey.pem
	fi
done

