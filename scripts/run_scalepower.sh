#!/bin/bash

### Set the simulation-specific parameters
BASEDIR=/home/bweel/experiments/scalepower/
TEMPLATEDIR=scripts/templates/
CONFNAME=scalepower_p
TESTNAME=scalepower_p

. ${BASEDIR}scripts/shflags

DEFINE_integer 'repeats' '50' 'The number of times the simulation should be run' 'X';
DEFINE_string 'robots' '10' 'The number of robots in the simulation' 'n';
DEFINE_string 'seed' '-1' 'Seed' 'x';
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

RANDOM=42

MU=3
ROBOTS=10
MUTATIONRATE=0.058071352541447
CROSSOVERRATE=0.993228793144226
REEVALUATIONRATE=0.567359387874603
STEPSPERCANDIDATE=800
INITIALMUTATIONSTEPSIZE=0.01

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

### Run the simulation
COUNTER=${FLAGS_repeats}

echo starting simulations, repeating $COUNTER times
cd $BASEDIR
until [ $COUNTER -lt 1 ];  do
	SEED=$RANDOM
	let COUNTER-=1
	let EXPERIMENT=${FLAGS_repeats}-$COUNTER
	echo "Running experiment #$EXPERIMENT with parameters: --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --seed $SEED"
	. ${BASEDIR}scripts/run_scale.sh --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --seed $SEED &

	SEED=$RANDOM
	let COUNTER-=1
	let EXPERIMENT=${FLAGS_repeats}-$COUNTER
	echo "Running experiment #$EXPERIMENT with parameters: --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --seed $SEED"
	. ${BASEDIR}scripts/run_scale.sh --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --seed $SEED

done

