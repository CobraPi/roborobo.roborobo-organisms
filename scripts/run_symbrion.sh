#!/bin/bash

### Set the simulation-specific parameters
DIR=$(cd "$(dirname "$0")"; pwd -P)
BASEDIR="$(dirname $DIR)/"
TEMPLATEDIR=scripts/templates/
CONFNAME=symbrion_p
TESTNAME=symbrion_p

FULLCOMMAND="$0 $@"
. ${BASEDIR}/scripts/shflags

#define the flags
DEFINE_integer 'repeats' '40' 'The number of times the simulation should be run';
DEFINE_string 'robots' '10' 'The number of robots in the simulation' 'n';
DEFINE_integer 'seed' '-1' 'Seed' 'x';
DEFINE_string 'iterations' '100000' 'Number of iterations' 'i'
DEFINE_string 'maxEggLifetime' '5000' 'Connection Type'
DEFINE_string 'maxSeedLifetime' '1000' 'Connection Type'
DEFINE_string 'maxOrganismLifetime' '5000' 'Connection Type'
DEFINE_string 'maxSeedBoardcastRange' '100' 'Connection Type'
DEFINE_string 'maxOrganismBoardcastRange' '100' 'Connection Type'
DEFINE_string 'minOrganismSize' '2' 'Connection Type'
DEFINE_string 'maxOrganismSize' '15' 'Connection Type'
DEFINE_string 'connectionType' '0' 'Connection Type'


#maxEggLifetimes=( 500 1000 1500 2000 2500 3000 3500 4000 4500 5000 5500 6000 6500 7000 7500 8000 8500 9000 9500 10000 )
maxEggLifetimes=( 4000)
maxSeedLifetimes=( 5000 )
maxOrganismLifetimes=( 17000 )

maxSeedBoardcastRange=100
maxOrganismBoardcastRange=100

minOrganismSize=2
maxOrganismSize=10

connectionType=7

RANDOM=37492093

ROBOTS=100
ITERATIONS=100000

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

### Run the simulation

run_roborobo() {
    echo "Running experiment #$EXPERIMENT with parameters: --maxEggLifetime $1 --maxSeedLifetime $2 --maxOrganismLifetime $3 --maxSeedBoardcastRange $maxSeedBoardcastRange --maxOrganismBoardcastRange $maxOrganismBoardcastRange --minOrganismSize $minOrganismSize --maxOrganismSize $maxOrganismSize --connectionType $connectionType --robots $ROBOTS --iterations $ITERATIONS --seed $SEED"
    . ${BASEDIR}scripts/run_egg.sh --maxEggLifetime $1 --maxSeedLifetime $2 --maxOrganismLifetime $3 --maxSeedBoardcastRange $maxSeedBoardcastRange --maxOrganismBoardcastRange $maxOrganismBoardcastRange --minOrganismSize $minOrganismSize --maxOrganismSize $maxOrganismSize --connectionType $connectionType --robots $ROBOTS --iterations $ITERATIONS --seed $SEED  > /dev/null 2> /dev/null
}

COUNTER=${FLAGS_repeats}

echo "starting simulations, repeating $COUNTER times"
cd $BASEDIR

cpu=20
MIN=1

for e in "${maxEggLifetimes[@]}"
do
:
    for s in "${maxSeedLifetimes[@]}"
    do
    :
        for o in "${maxOrganismLifetimes[@]}"
        do
        :
#            s=$e
#            o=$(($e*10))
            LOGDIR="logs/$e-$s-$o";
            echo "Logging to: $LOGDIR"
            mkdir -pv ${LOGDIR}
            COUNTER=${FLAGS_repeats}

            until [ "$COUNTER" -lt "$MIN" ];  do
                count=$(jobs | wc -l)
                if [ $(($count-$cpu)) -lt 0 ]; then
                    SEED=$RANDOM
                    let COUNTER-=1
                    let EXPERIMENT=${FLAGS_repeats}-$COUNTER
                    run_roborobo $e $s $o &
                fi
                sleep 2s
            done
        done
    done
done

wait
