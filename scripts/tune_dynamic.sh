#!/bin/bash

### Set the simulation-specific parameters
DIR=$(dirname $(readlink -f $0))
BASEDIR="$(dirname $DIR)/"
TEMPLATEDIR=scripts/templates/
CONFNAME=dynamic_p
TESTNAME=dynamic_p

FULLCOMMAND="$0 $@"
. ${BASEDIR}scripts/shflags

#define the flags
DEFINE_string 'robots' '10' 'The number of robots in the simulation' 'n';
DEFINE_integer 'seed' '-1' 'Seed' 'x';
DEFINE_string 'mu' '5' 'Mu' 'm'
DEFINE_string 'stepsPerCandidate' '950' 'Steps per candidate' 't'
DEFINE_string 'reEvaluationRate' '0.1' 'Re-evaluation Rate' 'r'
DEFINE_string 'crossoverRate' '1.0' 'Crossover rate' 'c'
DEFINE_string 'mutationRate' '1.0' 'Mutation rate' 'p'
DEFINE_string 'initialMutationStepSize' '5' 'Initial mutation step size' 's'
DEFINE_string 'newscastCacheSize' '4' 'Newscast cache size' 'C'
DEFINE_string 'newscastItemTTL' '8' 'Number of generations a Newscast item can live' 'L'
DEFINE_string 'numberOfParents' '2' 'Number of parents' 'P'
DEFINE_string 'iterations' '50000' 'Number of iterations' 'i'

ITERATIONS=1000
ROBOTS=100
STEPSPERCANDIDATE=950

cd $BASEDIR

echo "Running experiment #$EXPERIMENT with parameters: --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --newscastCacheSize ${FLAGS_newscastCacheSize} --newscastItemTTL ${FLAGS_newscastItemTTL} --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --iterations $ITERATIONS --speedScale ${FLAGS_speedScale} --seed $SEED --basedir $BASEDIR --templatedir $TEMPLATEDIR --confname $CONFNAME"
OUTPUT=$(${BASEDIR}scripts/run_scale.sh --robots $ROBOTS --mu $MU --stepsPerCandidate $STEPSPERCANDIDATE --mutationRate $MUTATIONRATE --crossoverRate $CROSSOVERRATE --reEvaluationRate $REEVALUATIONRATE --newscastCacheSize ${FLAGS_newscastCacheSize} --newscastItemTTL ${FLAGS_newscastItemTTL} --initialMutationStepSize $INITIALMUTATIONSTEPSIZE --iterations $ITERATIONS --speedScale ${FLAGS_speedScale} --seed $SEED --basedir $BASEDIR --templatedir $TEMPLATEDIR --confname $CONFNAME)


### compute some statistics
TICKSPERCONTROLLER=${FLAGS_stepsPerCandidate}
TICKSPERRANDOMFREE=0
TICKSPERMANUALFREE=50
TICKSPEREVAL=$((${TICKSPERCONTROLLER}+${TICKSPERRANDOMFREE}+${TICKSPERMANUALFREE}))

# extract the x most recent fitness values, where x is
# the number of evaluations that span 10% of total ticks
STEPS=$(echo "scale=0; ((${ITERATIONS}) * ${ROBOTS}) / ${TICKSPEREVAL}" | bc)
VALUES=$(tail -n $STEPS $OUTPUT | cut -f1 -d' ' | sed 's/.*e.*/0/')

# compute the average fitness value over the past 100% ticks
total=0
n=0

for fitness in $VALUES
do
  let n+=1
  total=$(echo "scale=6; $total + $fitness" | bc)
done

# write the average fitness value of the past 100% ticks to stdout and logfile
echo "scale=6; $total / $n" | bc
#echo `echo "scale=6; $total / $n" | bc` : $TIMESTAMP.$RID : $FULLCOMMAND >> commands.log
