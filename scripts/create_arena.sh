#!/bin/bash
# THIS SCRIPT REQUIRES IMAGEMAGICK

### Source shflags
#BASEDIR=/Users/$LOGNAME/Roborobo/
. ${BASEDIR}shflags

# Define the flags
DEFINE_string 'name' 'box4' 'The name of the basic arena image' 'n';
DEFINE_string 'x' '2' 'Horizontal repeats' 'x'
DEFINE_string 'y' '2' 'Vertical repeats' 'y'

# Parse the flags
FLAGS "$@" || exit 1
eval set -- "${FlAGS_ARGV}"

### Create the arena
INDIR=${BASEDIR}data/
OUTDIR=${INDIR}
INFILE=${INDIR}${FLAGS_name}.png
OUTFILE=${OUTDIR}${FLAGS_name}_${FLAGS_x}x${FLAGS_y}.png

# Do not overwrite the arena if it already exists
if [ -e $OUTFILE ]
then
	exit
fi

# Construct the command
COMMAND="convert \("
i="0"
while [ $i -lt ${FLAGS_x} ]
do
	i=$[$i+1]
	COMMAND="$COMMAND $INFILE"
done

COMMAND="$COMMAND +append \)"

i="1"
while [ $i -lt ${FLAGS_y} ]
do
	i=$[$i+1]
	COMMAND="$COMMAND +clone"
done

COMMAND="$COMMAND -append ${OUTFILE}"

#echo $COMMAND
eval $COMMAND
