#!/bin/bash

# run_parcours.sh
# roborobo
#
# Created by B.P.M. Weel on 3/20/12.
# Copyright 2012 VU. All rights reserved.

### Set the simulation-specific parameters
BASEDIR=/Users/bwl400/workspace/ahhs/
TEMPLATEDIR=scripts/templates/
CONFNAME=parcours_p
TESTNAME=parcours_p

FULLCOMMAND="$0 $@"
. ${BASEDIR}scripts/shflags

#define the flags
DEFINE_integer 'repeats' '50' 'The number of times the simulation should be run';
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
DEFINE_string 'iterations' '50000' 'Number of iterations' 'i'

RANDOM=42

MU=3
ROBOTS=50
MUTATIONRATE=0.4
CROSSOVERRATE=0.05
REEVALUATIONRATE=0.5
STEPSPERCANDIDATE=800
INITIALMUTATIONSTEPSIZE=0.1
ITERATIONS=2000000

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

### Run the simulation
COUNTER=${FLAGS_repeats}

echo starting simulations, repeating $COUNTER times
cd $BASEDIR
MIN=1


until [ "$COUNTER" -lt "$MIN" ];  do
	SEED=$RANDOM
	let COUNTER-=1
	let EXPERIMENT=${FLAGS_repeats}-$COUNTER
	echo "Running zxperiment #$EXPERIMENT with parameters: --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --iterations $ITERATIONS --seed $SEED"
	. ${BASEDIR}scripts/run_scale.sh --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --iterations $ITERATIONS --seed $SEED &

	SEED=$RANDOM
	let COUNTER-=1
	let EXPERIMENT=${FLAGS_repeats}-$COUNTER
	echo "Running experiment #$EXPERIMENT with parameters: --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --iterations $ITERATIONS --seed $SEED"
	. ${BASEDIR}scripts/run_scale.sh --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --iterations $ITERATIONS --seed $SEED
done
