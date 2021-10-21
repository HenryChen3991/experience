#!/bin/sh

# 正确
while read -N 3 data;do
  echo "$data"
done <data.txt

