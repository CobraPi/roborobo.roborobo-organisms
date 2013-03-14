#!/bin/bash

### Set the simulation-specific parameters
BASEDIR=/home/bweel/workspace/ahhs/
CONFNAME=ahhs_p
TESTNAME=ahhs_p

FULLCOMMAND="$0 $@"
. ${BASEDIR}shflags

#define the flags
DEFINE_string 'robots' '10' 'The number of robots in the simulation' 'n';
DEFINE_integer 'seed' '-1' 'Seed' 'x';
DEFINE_string 'mu' '5' 'Mu' 'm'
DEFINE_string 'stepsPerCandidate' '500' 'Steps per candidate' 't'
DEFINE_string 'reEvaluationRate' '0.1' 'Re-evaluation Rate' 'r'
DEFINE_string 'crossoverRate' '1.0' 'Crossover rate' 'c'
DEFINE_string 'mutationRate' '1.0' 'Mutation rate' 'p'
DEFINE_string 'initialMutationStepSize' '5' 'Initial mutation step size' 's'
DEFINE_string 'newscastCacheSize' '4' 'Newscast cache size' 'C'
DEFINE_string 'newscastItemTTL' '8' 'Number of generations a Newscast item can live' 'L'
DEFINE_string 'numberOfParents' '2' 'Number of parents' 'P'
DEFINE_string 'startHormones' '2' 'Number of start hormones' 'H'
DEFINE_string 'startRules' '20' 'Number of start rules' 'R'

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

cd $BASEDIR
. ${BASEDIR}run_scale.sh
