#!/bin/bash

### Set the simulation-specific parameters
BASEDIR=/home/bweel/RoboRobo/
CONFNAME=mpo_p
TESTNAME=mpo_p2
ARENANAME=box4
ARENAZONESNAME=box4_zones
ARENA_SIZE=500
offsetX=25
offsetY=25

### Run the simulation
cd $BASEDIR
. ${BASEDIR}run_general.sh
