#!/bin/bash

FULLCOMMAND="$0 $@"
. ${BASEDIR}scripts/shflags

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
DEFINE_string 'iterations' '50000' 'Number of iterations' 'i'
DEFINE_string 'scaleFactor' '4' 'Scale of organism speed increase' 'a'

DEFINE_string 'basedir' '/home/bweel/' 'Base dir of experiment' 'b'
DEFINE_string 'templatedir' 'scripts/template' 'd'
DEFINE_string 'confname' 'scalepower_p' 'o'

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

BASEDIR=${FLAGS_basedir}
TEMPLATEDIR=${FLAGS_templatedir}
CONFNAME=${FLAGS_confname}

RID=$RANDOM

### copy the template configuration to the config dir, making the neccesary adjustments
# Determine some properties of the test
ROBOTCOUNT=${FLAGS_robots}
TIMESTAMP=`date "+%Y%m%d.%Hh%Mm%Ss"`

# Determine where the configuration file will be placed
CONFDIR=${BASEDIR}config/
CONFFILE=${CONFDIR}${TIMESTAMP}.$RID.${TESTNAME}.$ROBOTCOUNT.properties
LOGFILE=${BASEDIR}/logs/console-output.${TIMESTAMP}.$RID.log
ERRORLOGFILE=${BASEDIR}/logs/console-error.${TIMESTAMP}.$RID.log

OUTPUTLOGFILE=logs\\/output.${TIMESTAMP}.$RID.log
GENOMELOGFILE=logs\\/genomes.${TIMESTAMP}.$RID.log
ORGANISMSIZESLOGFILE=logs\\/organism-sizes.${TIMESTAMP}.$RID.log
ORGANISMSLOGFILE=logs\\/organisms.${TIMESTAMP}.$RID.log
LOCATIONLOGFILE=logs\\/locations.${TIMESTAMP}.$RID.log

# Prepare the rules that will fill out the configuration file
SEEDREP=s/--RANDOMSEED/${FLAGS_seed:0:9}/g # extract only the first 9 decimals, because Roborobo can't handle int overflows
NBAGENTSREP=s/--NBAGENTS/${ROBOTCOUNT}/g
MUREP=s/--MU/${FLAGS_mu}/g
SPCREP=s/--STEPSPERCANDIDATE/${FLAGS_stepsPerCandidate}/g
RERREP=s/--REEVALUATIONRATE/${FLAGS_reEvaluationRate}/g
CRREP=s/--CROSSOVERRATE/${FLAGS_crossoverRate}/g
MRREP=s/--M-TATIONRATE/${FLAGS_mutationRate}/g
IMSSREP=s/--INITIALMUTATIONSTEPSIZE/${FLAGS_initialMutationStepSize}/g
NCCREP=s/--NEWSCASTCACHESIZE/${FLAGS_newscastCacheSize}/g
NCTTLREP=s/--NEWSCASTITEMTTL/${FLAGS_newscastItemTTL}/g
NOPREP=s/--NUMBEROFPARENTS/${FLAGS_numberOfParents}/g
ITERATIONREP=s/--ITERATIONS/${FLAGS_iterations}/g
SCALEFACTORREP=s/--SCALEFACTOR/${FLAGS_scaleFactor}/g

OUTPUTLOGREP=s/--OUTPUTLOG/${OUTPUTLOGFILE}/g
GENOMELOGREP=s/--GENOMELOG/${GENOMELOGFILE}/g
ORGANISMSIZESREP=s/--ORGANISMSIZESLOG/${ORGANISMSIZESLOGFILE}/g
LOCATIONREP=s/--LOCATIONLOG/${LOCATIONLOGFILE}/g

# Fill out and place the configuration file
sed -e $SEEDREP -e $NBAGENTSREP -e $MUREP -e $SPCREP -e $RERREP -e $CRREP -e $MRREP -e $IMSSREP -e $NCCREP -e $NCTTLREP -e $OUTPUTLOGREP -e $GENOMELOGREP -e $ORGANISMSIZESREP -e $NOPREP -e $ITERATIONREP -e $LOCATIONREP -e $SCALEFACTORREP ${BASEDIR}${TEMPLATEDIR}${CONFNAME}.template.properties > $CONFFILE

if [ $? -ne 0 ]
then
    exit $?
fi


### Run RoboRobo!
BINFILE=${BASEDIR}roborobo
$BINFILE -l $CONFFILE > $LOGFILE 2> $ERRORLOGFILE 
