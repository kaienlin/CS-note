#! /usr/bin/env bash

N=10

usage() {
	echo "Usage: csie_analytics.sh [-n count] [filename]:"
}

if [ $# -eq 0 ]; then
	usage  # lack file name
	exit 5
fi

if [ "$1" == "-n" ]; then
	if [ $# -eq 2 ]; then
		if [ -f "$2" ]; then   # example: ./csie_analytics.sh -n log.txt
			echo "Error: option requires an argument."
			exit 1 
		else  # example: ./csie_analytics.sh -n 8
			usage
			exit 5
		fi
	elif [ `echo "$2" | grep -c "^[1-9][0-9]*$"` -eq 0 ]; then  # example: ./csie_analytics.sh -n 11.7 log.txt
		echo "Error: line number must be positive integer."
		exit 2
	else  # correct: ./csie_analytics -n 42 log.txt
		FILENAME=$3
		N=$2
	fi
elif [ `echo "$1" | grep -c -- '-.*'` -ne 0 ] || [ $# -gt 1 ]; then  # example: ./csie_analytics.sh -q ... || ./csie_analytics.sh log.txt -n 999
	usage  # Other arguments error
	exit 3
else
	FILENAME="$1"  # correct or file not exists
fi

if [ ! -f "$FILENAME" ]; then
	echo "Error: log file does not exist."
	exit 4
fi

data=`cat $FILENAME | cut -d ' ' -f 5 | sed 's/?[^ ]*//g' | sort | uniq -c | sort -gr -k 1`
sum=`cat "$FILENAME" | wc -l`

echo "$data" | head -n $N | awk -v sum="$sum" 'BEGIN{printf "%-35s %-10s %s\n", "Path", "Times", "Percentage"} {percentage = $1 / sum * 100; printf "%-35s %-10s %-2.2f%%\n", $2, $1, percentage}'

