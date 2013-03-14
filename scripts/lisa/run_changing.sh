#!/bin/sh

#  run_changing.sh
#  roborobo
#
#  Created by Berend Weel on 10/24/12.
#
#!/bin/bash

echo "starting job submissions"

scaleFactors=( "0,2,0,-2,0" "0,10,0,-10,0" "0,5,10,5,0,-5,-10,-5,0" "0,5,0,5,0,10,0,10,0" )
repeats=40

REPEATREP=s/--REPEATS/${repeats}/g

for sf in "${scaleFactors[@]}"
do
:
    SCALEREP=s/--SCALEFACTOR/${sf}/g
    scriptName=run_changingworld-$sf.sh
    sed -e $REPEATREP -e $SCALEREP run_changingworld.sh > $scriptName
    chmod +x $scriptName

    echo "Submitting $scriptName to the queue"
    qsub $scriptName
done
