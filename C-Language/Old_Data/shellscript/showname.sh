#!/bin/bash
#Program:
#    User inputs his first name and last name. Program show his full name
#History:
#    2018/01/31
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/loacl/sbin:~/bin
export PATH

read -p "Please input your first name: " firstName
read -p "Please input your last name: " lastName
echo -e "\nYour full name is: ${firstName} ${lastName}"

