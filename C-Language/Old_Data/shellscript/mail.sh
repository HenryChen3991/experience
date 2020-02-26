#!/bin/bash
#test send email by shell scirpt to notice something

#set email address
henry_email="ckl19930819@hotmail.com"

echo "shell script" | mail -s test "$heney_email"

#should install mail command
#ubuntu command
#sudo apt-get install mailutils
