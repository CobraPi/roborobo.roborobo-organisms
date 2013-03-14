#!/bin/bash

DIR=$1
ARRAYSIZE=$2 # default was 100
GEN=$3 # default was 198

for i in "$DIR"/datalog*.txt
do
	echo $i
	#cat $i | grep monitorOrientation | sed 's/monitorOrientation:\ //' | sort -n -t, -k3,3n -k4,4n | tail -n$ARRAYSIZE
done


for it in {0..99}
do	
	echo $it
	cat $DIR/datalog_*.txt | grep monitorOrientation | sed 's/monitorOrientation:\ //' | egrep '[[:digit:]]{1,3}\,[[:digit:]]{1,3}\,`${{GEN}}}`\,[[:digit:]]{1,3}\,[[:digit:]]{1,3}'


 # TARGET::: | egrep  [[:digit:]]{1,3}\,[[:digit:]]{1,3}\,$GEN\,$it\,[[:digit:]]{1,3}

	#cat "$DIR"/datalog_*.txt | grep monitorOrientation | sed 's/monitorOrientation:\ //' | egrep '[[:digit:]]{1,3}\,[[:digit:]]{1,3}\,$"GEN",$"it"\,[[:digit:]]{1,3}'
done

