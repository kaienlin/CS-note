#! /usr/bin/env bash

date_patterns=('^[0-9]{4}-[0-9]{2}-[0-9]{2}$'
			   '^[0-9]{4}/[0-9]{2}/[0-9]{2}$'
			   '^[0-9]{4}\.[0-9]{2}\.[0-9]{2}$')
time_patterns=('^[0-9]{2}:[0-9]{2}$'
			   '^[0-9]{2}:[0-9]{2}:[0-9]{2}$')

date_is_valid=0
time_is_valid=0

for pattern in ${date_patterns[@]}; do
	if [ `echo "$1" | grep -cE "$pattern"` -eq 1 ]; then
		date_is_valid=1
	fi
done

for pattern in ${time_patterns[@]}; do
	if [ `echo "$2" | grep -cE "$pattern"` -eq 1 ]; then
		time_is_valid=1
	fi
done

if [ $date_is_valid != 0 ] && [ $time_is_valid != 0 ]; then
	echo "$1 $2"
else
	echo "Invalid"
fi

