#!/bin/sh

OBJ="FR51782"


make clean

make

./omci2pcap $OBJ.txt  $OBJ.pcap.format

echo -e "\nOmci.pcap:"
#cat omci.pcap.format

echo -e "\ntext2pcap $OBJ.pcap.format $OBJ.pcap:"
text2pcap $OBJ.pcap.format $OBJ.pcap

mkdir -p exe-$OBJ
mv $OBJ.* exe-$OBJ
