#!/bin/bash

float_scale=2

function float_eval()
{
	local stat=0
		local result=0.0
		if [[ $# -gt 0 ]]; then
			result=$(echo "scale=$float_scale; $*" | bc -q 2>/dev/null)
				stat=$?
				if [[ $stat -eq 0  &&  -z "$result" ]]; then stat=1; fi
					fi
						echo $result
						return $stat
}


cd Roborobo11
mkdir results

	for i in `seq 7 11`
	do
		
		mkdir results/exp$i
		echo "***************************"
		echo "lancement de l'expérience $i"
		echo "***************************"
		./roborobo
		echo "***************************"
		echo "déplacement du fichier de log"
		echo "***************************"
		mv logs/log.txt results/exp$i/log_Algo.log
		mv logs/debug.properties results/exp$i/debug.properties
	done

echo "***************************"
echo "C'est la fin !!!"
echo "***************************"
