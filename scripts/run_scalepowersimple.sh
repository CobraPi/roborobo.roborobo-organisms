#!/bin/bash

### Set the simulation-specific parameters
BASEDIR=/home/bwl400/scratch/RoboRobo/trunk/
CONFNAME=scalepower_simple_p
TESTNAME=scalepower_simple_p

### Run the simulation
cd $BASEDIR
. ${BASEDIR}run_scale.sh
