#! /bin/tcsh

set cntInit=10
set cntMax=80
set step=1

# 1 - extract data

set cnt=$cntInit

while ( $cnt <= $cntMax )
	rm -f tmp.o.rawdata.${cnt}_16.tmp
	cat /dev/null > tmp.o.rawdata.${cnt}_16.tmp
	rm -f tmp.o.data.${cnt}_16.tmp
	cat /dev/null > tmp.o.data.${cnt}_16.tmp
	rm -f tmp.o.rawdata.${cnt}_32.tmp
	cat /dev/null > tmp.o.rawdata.${cnt}_32.tmp
	rm -f tmp.o.data.${cnt}_32.tmp
	cat /dev/null > tmp.o.data.${cnt}_32.tmp
	rm -f tmp.o.rawdata.${cnt}_64.tmp
	cat /dev/null > tmp.o.rawdata.${cnt}_64.tmp
	rm -f tmp.o.data.${cnt}_64.tmp
	cat /dev/null > tmp.o.data.${cnt}_64.tmp
	@ cnt = $cnt + $step
end

@ cnt = $cntInit

while ( $cnt <= $cntMax )
	echo generating raw data $cnt / $cntMax
	./extractTargetOrientation.sh  exp_${cnt}_16/ 100 198 >> tmp.o.rawdata.${cnt}_16.tmp
	./extractTargetOrientation.sh  exp_${cnt}_32/ 100 198 >> tmp.o.rawdata.${cnt}_32.tmp
	./extractTargetOrientation.sh  exp_${cnt}_64/ 100 198 >> tmp.o.rawdata.${cnt}_64.tmp
	
	@ cnt = $cnt + $step
end

# 2 - aggregate data

@ cnt = $cntInit

while ( $cnt <= $cntMax )
	echo generating clean data $cnt / $cntMax
	python aggregateValues.py tmp.o.rawdata.${cnt}_16.tmp >> tmp.o.data.${cnt}_16.tmp
	python aggregateValues.py tmp.o.rawdata.${cnt}_32.tmp >> tmp.o.data.${cnt}_32.tmp
	python aggregateValues.py tmp.o.rawdata.${cnt}_64.tmp >> tmp.o.data.${cnt}_64.tmp
	@ cnt = $cnt + $step
end


# 3 - trace
