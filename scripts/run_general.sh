#!/bin/bash

### MAKE SURE TO INCLUDE THIS FILE FROM ANOTHER WHERE YOU SET THE SIMULATION-SPECIFIC PARAMETERS

### Source shflags
FULLCOMMAND="$0 $@"
. ${BASEDIR}shflags
RID=$RANDOM

# Define the flags
DEFINE_string 'robots' '4' 'The number of robots in the simulation' 'n';
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

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

### copy the template configuration to the config dir, making the neccesary adjustments
# Determine some properties of the test
ROBOTCOUNT=${FLAGS_robots}
ARENAS_MULTIPLY=$(echo "ceil(sqrt(${ROBOTCOUNT}))" | bc funcs.bc)
TIMESTAMP=`date "+%Y%m%d.%Hh%Mm%Ss"`

# Determine where the configuration file will be placed
CONFDIR=${BASEDIR}config/
CONFFILE=${CONFDIR}${TIMESTAMP}.$RID.${TESTNAME}.$ROBOTCOUNT.properties
LOGFILE=output.${TIMESTAMP}.$RID.log

# Determine where the arena files will be placed
DATADIR=${BASEDIR}data/
DATADIR_ESC=data\\/
ARENAINFILE=${DATADIR}${ARENANAME}.png
ARENAOUTFILE_ESC=${DATADIR_ESC}${ARENANAME}_${ARENAS_MULTIPLY}x${ARENAS_MULTIPLY}\\.png
ARENAZONESOUTFILE_ESC=${DATADIR_ESC}${ARENAZONESNAME}_${ARENAS_MULTIPLY}x${ARENAS_MULTIPLY}\\.png

# Prepare the rules that will fill out the configuration file
ARENAREP=s/--ARENA/${ARENAOUTFILE_ESC}/g
ZONESREP=s/--ZONES/${ARENAZONESOUTFILE_ESC}/g
SEEDREP=s/--SEED/${FLAGS_seed:0:9}/g # extract only the first 9 decimals, because Roborobo can't handle int overflows
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

# Fill out and place the configuration file
sed -e $ARENAREP -e $ZONESREP -e $SEEDREP -e $NBAGENTSREP -e $MUREP -e $SPCREP -e $RERREP -e $CRREP -e $MRREP -e $IMSSREP -e $NCCREP -e $NCTTLREP -e $NOPREP ${BASEDIR}${CONFNAME}.template.properties > $CONFFILE

### Modify the configuration to position the correct number of robots
i=0
agentNR=0
while [ "$i" -lt $ARENAS_MULTIPLY ]
do
	j="0"
	while [ "$j" -lt $ARENAS_MULTIPLY ]
	do
		AGENTNRREP=s/--AGENTNR/$agentNR/g
		agentX=$(($j*$ARENA_SIZE + $offsetX))
		AGENTXREP=s/--AGENTX/$agentX/g
		agentY=$(($i*$ARENA_SIZE + $offsetY))
		AGENTYREP=s/--AGENTY/$agentY/g
		
		sed -e $AGENTNRREP -e $AGENTXREP -e $AGENTYREP ${BASEDIR}robotpos.template.properties >> $CONFFILE
		
		j=$[$j+1]
		agentNR=$[$agentNR+1]
	done
	i=$[$i+1]
done

### Create the arena files
./create_arena.sh --name $ARENANAME --x $ARENAS_MULTIPLY --y $ARENAS_MULTIPLY
./create_arena.sh --name $ARENAZONESNAME --x $ARENAS_MULTIPLY --y $ARENAS_MULTIPLY

### Run RoboRobo!
BINFILE=${BASEDIR}roborobo
$BINFILE -l $CONFFILE > $LOGFILE

### compute some statistics
TICKSPERCONTROLLER=${FLAGS_stepsPerCandidate}
TICKSPERRANDOMFREE=10
TICKSPERMANUALFREE=150
TICKSPEREVAL=$((${TICKSPERCONTROLLER}+${TICKSPERRANDOMFREE}+${TICKSPERMANUALFREE}))

# extract the x most recent fitness values, where x is
# the number of evaluations that span 5.000 ticks (* Y, because Y robots) (10% of total)
steps=$(echo "scale=0; 5000 * ${ROBOTCOUNT} / ${TICKSPEREVAL}" | bc)
values=`tail -n $steps $LOGFILE | cut -f1 -d' ' | sed 's/.*e.*/0/'`

# compute the average fitness value over the past 5.000 ticks
total=0
n=0

for fitness in $values
do
  n=$(( $n+1 ))
  total=$(echo "scale=6; $total + $fitness" | bc)
done

# write the average fitness value of the past 5.000 ticks to stdout and logfile
echo "scale=6; $total / $n" | bc
echo `echo "scale=6; $total / $n" | bc` : $TIMESTAMP.$RID : $FULLCOMMAND >> commands.log
