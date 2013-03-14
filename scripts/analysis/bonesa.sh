#!/bin/bash

if [ -z "$1" ]
then
	echo "Please provide the number of lines in the file to digest"
	exit
fi

if [ -z "$2" ]
then
	echo "Please provide the filename of the file to digest"
	exit
fi

if [ ! -f "$2" ]
then
	echo "File to digest does not exist"
	exit
fi

head -n $1 $2 | sort -n | tail -n 20

