#! /bin/tcsh

set cnt=10
set cntMax=80
set step=1

while ( $cnt <= $cntMax )

	cp config_template.txt config_${cnt}_16.properties
	echo gNbOfAgents = $cnt >> config_${cnt}_16.properties
	echo gMaxRadioDistance = 16 >> config_${cnt}_16.properties
	echo ./roborobo -l config/config_${cnt}_16.properties
	echo mkdir logs/exp_${cnt}_16_sun10
	echo "mv logs/datalog* logs/exp_${cnt}_16_sun10/"
	echo "mv logs/properties* logs/exp_${cnt}_16_sun10/"
	echo "mv logs/screenshot* logs/exp_${cnt}_16_sun10/"

	cp config_template.txt config_${cnt}_32.properties
	echo gNbOfAgents = $cnt >> config_${cnt}_32.properties
	echo gMaxRadioDistance = 32 >> config_${cnt}_32.properties
	echo ./roborobo -l config/config_${cnt}_32.properties
	echo "mkdir logs/exp_${cnt}_32_sun10"
        echo "mv logs/datalog* logs/exp_${cnt}_32_sun10/"
        echo "mv logs/properties* logs/exp_${cnt}_32_sun10/"
        echo "mv logs/screenshot* logs/exp_${cnt}_32_sun10/"

	cp config_template.txt config_${cnt}_64.properties
	echo gNbOfAgents = $cnt >> config_${cnt}_64.properties
	echo gMaxRadioDistance = 64 >> config_${cnt}_64.properties
	echo ./roborobo -l config/config_${cnt}_64.properties
	echo "mkdir logs/exp_${cnt}_64_sun10"
        echo "mv logs/datalog* logs/exp_${cnt}_64_sun10/"
        echo "mv logs/properties* logs/exp_${cnt}_64_sun10/"
        echo "mv logs/screenshot* logs/exp_${cnt}_64_sun10/"

	#@ cnt++
	@ cnt = $cnt + $step
end


