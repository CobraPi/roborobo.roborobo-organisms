#!/bin/bash

### Set the simulation-specific parameters
BASEDIR=/Users/$LOGNAME/Roborobo/
CONFNAME=hivemind_p
TESTNAME=hivemind_p2
ARENANAME=box4
ARENAZONESNAME=box4_zones
ARENA_SIZE=500
offsetX=25
offsetY=25

### Run the simulation
cd $BASEDIR
. ${BASEDIR}run_general.sh
