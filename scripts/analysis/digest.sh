#!/bin/bash

if [ -z "$1" ]
then
	echo "Please provide the number of lines in the file to digest"
	exit
fi

if [ -z "$2" ]
then
	echo "Please provide the number of samples per setting to include (0 for all)"
	exit
fi

if [ -z "$3" ]
then
	echo "Please provide the filename of the file to digest"
	exit
fi

if [ ! -f "$3" ]
then
	echo "File to digest does not exist"
	exit
fi

head -n $1 $3 | sort -n | ./combine.awk -v maxCount=$2 | sort -n | tail -n 20

#cat $2 | sort -n --key=5 | ./combine.awk -v maxCount=$1 | sort -n | tail -n 20
