#!/usr/bin/python
# 2010 04 20 - jeanmarc.montanier(at)lri.fr


import glob
import os
import getopt
import re
import commands

import datetime
import sys

from optparse import OptionParser
from shutil import *


usage = "usage %prog [options]"
parser = OptionParser()
parser.add_option("-F","--file",dest="logFile",help="The log file that has to bee analyzed by the script (default value = none)", default="none")
parser.add_option("-g","--generation",dest="generation",help="Define at which generation the specified robot should be searched (default value = 1)", metavar="GENERATION", default="1")
parser.add_option("-l","--lifeTime",dest="lifeTime",help="life time of the robots in the considered experiments (default = 400)", metavar="LIFETIME", default="400")
parser.add_option("-r","--robot",dest="robot",help="Define in which robot the script should fetch the genome that has to be displayed (default = 0)", metavar="ROBOT", default="0")
parser.add_option("-t","--lengthReplay",dest="lengthReplay",help="length of the replay in number of iteration (default 1000).", metavar="LENGTHREPLAY", default="1000")
parser.add_option("-x","--expSetup",dest="expSetup",help="experimental setup used for this replay (default 0).", metavar="EXPERIENCE_REPLAY", default="0")

(options, args) = parser.parse_args()

if (not options.logFile == "none"):
	if os.path.isfile(options.logFile):
		filenames=[options.logFile]
	else:
		parser.error("The path ("+ str(options.logFile) + ") doesn't lead to any file")
else:
	parser.error("No log file given. This script refuse to work in those conditions !!")

print '#'
print '# Replay the behavior of the specifed robot'
print '# ' + str(datetime.datetime.now().ctime())
print '#'
print '# source(s):'
for filename in filenames:
    print '#\t' + filename + '\n',
print '# Generation :'
print '# ' + str(options.generation)
print '# LifeTime :'
print '# ' + str(options.lifeTime)
print '# Robot :'
print '# ' + str(options.robot)
print '# LengthReplay :'
print '# ' + str(options.lengthReplay)
print '# ExpSetup :'
print '# ' + str(options.expSetup)
print '#'



fileContentArray = open(filename,'r').readlines()
filePath = os.path.dirname(filename)

print "Searching the holly robot in the log file : " + str(datetime.datetime.now().ctime())

genomeToReplay = []

for line in fileContentArray:
	patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) use genome :(.*)"
	patternGenome = re.compile(patternDescription)
	genomeMatch = patternGenome.match(line)
	if(genomeMatch):
		idIteration = genomeMatch.group(1)
		idBot = genomeMatch.group(2)
		generation = (int(idIteration)+1) / int(options.lifeTime)
		if ( ( generation == int(options.generation) ) and ( int(idBot) == int(options.robot) ) ):
			genome = re.split(" ",genomeMatch.group(3))
			genome = genome[:-1]
			genomeToReplay = genome
			break

	patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) is waiting for a new genome"
	patternGenome = re.compile(patternDescription)
	genomeMatch = patternGenome.match(line)
	if(genomeMatch):
		idIteration = genomeMatch.group(1)
		idBot = genomeMatch.group(2)
		generation = (int(idIteration)+1) / int(options.lifeTime)
		if ( ( generation == int(options.generation) ) and ( int(idBot) == int(options.robot) ) ):
			print "The holly robot is empty"
			exit(1)

if ( genomeToReplay == [] ):
	print "The holly robot doesn't exist"
	exit(1)


copyfile('replayHead','./config/Replay.properties')
replayFile = open('./config/Replay.properties',"a")
replayFile.write('gMaxIt=  ' + options.lengthReplay + '\n' )
replayFile.write('gExperimentNumber=  ' + options.expSetup + '\n' )

geneCpt = 0
for gene in genomeToReplay:
	replayFile.write('genomeToReplay['+str(geneCpt)+'] = '+gene+'\n')
	geneCpt +=1

replayFile.close()

print "launchBattle the replay : " + str(datetime.datetime.now().ctime())

battleOutput = commands.getoutput("./roborobo -l config/Replay.properties")	
print battleOutput
