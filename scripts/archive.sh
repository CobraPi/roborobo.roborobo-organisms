#!/bin/bash

TIMESTAMP=`date "+%Y%m%d.%Hh%Mm%Ss"`
BASEDIR=../
RESULTDIR=${BASEDIR}results/v8/$TIMESTAMP
mkdir $RESULTDIR
mkdir $RESULTDIR/config
mkdir $RESULTDIR/logs
mkdir $RESULTDIR/fitness
mkdir $RESULTDIR/organisms
mkdir $RESULTDIR/organismsizes
mkdir $RESULTDIR/output
mkdir $RESULTDIR/genomes

mv ${BASEDIR}output.* $RESULTDIR/fitness/
mv ${BASEDIR}commands.log $RESULTDIR

mv ${BASEDIR}config/201*h*m*s.*.*.properties $RESULTDIR/config/

mv ${BASEDIR}logs/properties_*.txt $RESULTDIR/logs/
mv ${BASEDIR}logs/genomes.*.log $RESULTDIR/genomes/
mv ${BASEDIR}logs/organisms.*.log $RESULTDIR/organisms/
mv ${BASEDIR}logs/organism-sizes.*.log $RESULTDIR/organismsizes/
mv ${BASEDIR}logs/output.*.log $RESULTDIR/output
