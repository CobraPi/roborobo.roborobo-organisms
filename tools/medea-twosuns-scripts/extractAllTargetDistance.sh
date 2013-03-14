#! /bin/tcsh

set cntInit=10
set cntMax=80
set step=1

# 1 - extract data

set cnt=$cntInit

while ( $cnt <= $cntMax )
	rm -f tmp.d.rawdata.${cnt}_16.tmp
	cat /dev/null > tmp.d.rawdata.${cnt}_16.tmp
	rm -f tmp.d.data.${cnt}_16.tmp
	cat /dev/null > tmp.d.data.${cnt}_16.tmp
	rm -f tmp.d.rawdata.${cnt}_32.tmp
	cat /dev/null > tmp.d.rawdata.${cnt}_32.tmp
	rm -f tmp.d.data.${cnt}_32.tmp
	cat /dev/null > tmp.d.data.${cnt}_32.tmp
	rm -f tmp.d.rawdata.${cnt}_64.tmp
	cat /dev/null > tmp.d.rawdata.${cnt}_64.tmp
	rm -f tmp.d.data.${cnt}_64.tmp
	cat /dev/null > tmp.d.data.${cnt}_64.tmp
	@ cnt = $cnt + $step
end

@ cnt = $cntInit

while ( $cnt <= $cntMax )
	echo generating raw data $cnt / $cntMax
	./extractTargetDistance.sh  exp_${cnt}_16/ 100 198 >> tmp.d.rawdata.${cnt}_16.tmp
	./extractTargetDistance.sh  exp_${cnt}_32/ 100 198 >> tmp.d.rawdata.${cnt}_32.tmp
	./extractTargetDistance.sh  exp_${cnt}_64/ 100 198 >> tmp.d.rawdata.${cnt}_64.tmp
	
	@ cnt = $cnt + $step
end

# 2 - aggregate data

@ cnt = $cntInit

while ( $cnt <= $cntMax )
	echo generating clean data $cnt / $cntMax
	python aggregateValues.py tmp.d.rawdata.${cnt}_16.tmp >> tmp.d.data.${cnt}_16.tmp
	python aggregateValues.py tmp.d.rawdata.${cnt}_32.tmp >> tmp.d.data.${cnt}_32.tmp
	python aggregateValues.py tmp.d.rawdata.${cnt}_64.tmp >> tmp.d.data.${cnt}_64.tmp
	@ cnt = $cnt + $step
end


# 3 - trace
