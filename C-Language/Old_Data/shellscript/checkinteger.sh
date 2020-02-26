
#!/bin/sh
 
VAR=${1}
 
echo "${VAR}" | grep "[^0-9]" > /dev/null >&1
if [ "$?" -eq "0" ]; then
   echo string
else
   echo integer
fi
