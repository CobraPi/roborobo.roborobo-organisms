#! /bin/tcsh

if ($# < 2) then
echo USAGE: $0 path_to_source_experiment path_to_target
else

set cntInit=10
set cnt=$cntInit
set cntMax=80
set step=1

mkdir $2

while ( $cnt <= $cntMax )

	mkdir $2/exp_${cnt}_16
	mkdir $2/exp_${cnt}_32
	mkdir $2/exp_${cnt}_64

	@ cnt = $cnt + $step
end

@ cnt = $cntInit

while ( $cnt <= $cntMax )

	cp -v $1/logs/exp_${cnt}_16/* $2/exp_${cnt}_16/
	cp -v $1/logs/exp_${cnt}_32/* $2/exp_${cnt}_32/
	cp -v $1/logs/exp_${cnt}_64/* $2/exp_${cnt}_64/

	@ cnt = $cnt + $step
end

endif


