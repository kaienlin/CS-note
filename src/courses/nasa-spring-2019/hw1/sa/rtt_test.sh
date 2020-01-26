#! /usr/bin/env bash

output_str=''
url_list=`cat $1 | grep '^#Server = ' | cut -d ' ' -f 3`

for url in $url_list
do
	ping_result=`ping -c 3 -q "$url" 2>&1`
	#if [ `echo "$ping_result" | grep -E -c "not known|Unknown host|100% packet loss"` == '0' ]
	if [ $? -eq 0 ]
	then
		rtt=`echo "$ping_result" | grep rtt | cut -d ' ' -f 4 | cut -d '/' -f 2`
		output_str="$output_str$url $rtt\n"
	fi
done

printf "${output_str}" | sort -g -k 2

