#Run script for Bonesa on the Lisa system
#TO RUN:  1. Create a distributed package with Bonesa. Set the host
#            name to any string (it will not be used).
#         2. Copy the "dist" directory created by Bonesa to Lisa
#         3. Edit the directory/file settings below         
#         4. Copy the "autorun.properties" as "autorun" in the dist
#            directory. In "autorun" remove the line with bonesa.host
#            and change bonesa.waitwhenfinished to false.
#         5. Edit the PBS setttings below 

# Edit settings of PBS (do NOT uncomment these lines)
#--------------------------------------------------------------------------------------------------#
#PBS -lnodes=1:ppn=16
#PBS -lwalltime=10:00:00
#--------------------------------------------------------------------------------------------------#

### Set the simulation-specific parameters

# Edit the following directory/file settings
#--------------------------------------------------------------------------------------------------#
workdir=/home/bweel/experiments
roborobodir=/home/bweel/experiments/parcours/
mytmpdir=bweel #Subdirectory used in nodes' scratch space
#--------------------------------------------------------------------------------------------------#

repeats=--REPEATS
scaleFactor=--SCALEFACTOR

resultsdir="/home/bweel/experiments/results/parcours/$scaleFactor"

dumpdir=${workdir}/dumps 
scriptdump=${dumpdir}/script-${scaleFactor}

mkdir -pv $dumpdir

rm $scriptdump
rm $dumpdir/host_*
rm $dumpdir/node_*

#Init script dump
echo "$(date)" > $scriptdump
echo "Script running on `hostname`" >> $scriptdump

echo "Nodes reserved:" >> $scriptdump
NODES="$(sort $PBS_NODEFILE | uniq)"
echo "$NODES" >> $scriptdump

echo "Creating results dir: $resultsdir" >> $scriptdump
mkdir -pv $resultsdir

#Start master process on this host
echo "Re-creating tempdir..." >> $scriptdump
cd $TMPDIR;
rm -rf $mytmpdir
mkdir $mytmpdir
cd $mytmpdir

echo "Copying roborobo..." >> $scriptdump
rm -rf *
cp -rf ${roborobodir} ./
cd parcours

echo "Starting runs..." >> $scriptdump
./scripts/run_changingworld.sh --repeats $repeats --scaleFactor $scaleFactor >> $scriptdump 2>&1

#Wait until local process (master) is done
wait

#Copy result bda file from this host's scratch space to the results file in home directory
echo "Done! Copying results back..." >> $scriptdump
cp -rf $TMPDIR/$mytmpdir/parcours/logs/* ${resultsdir}
		
echo "End at $(date)" >> $scriptdump

