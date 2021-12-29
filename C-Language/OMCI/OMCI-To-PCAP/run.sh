#!/bin/sh

make clean

make

./omci2pcap omci.txt  omci.pcap.format

echo -e "\nOmci.pcap:"
#cat omci.pcap.format

echo -e "\ntext2pcap omci.pcap.format omci.pcap:"
text2pcap omci.pcap.format omci.pcap
