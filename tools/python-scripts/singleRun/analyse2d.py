#!/usr/bin/python
# 2009 02 23 - niko(at)lri.fr
# 2009 07 20 - jeanmarc.montanier(at)lri.fr
# History:
#  Started from the basic log analyse script of nicolas bredeche
#
# Syntax :
#  To know it type 'sortBest -h'
#
# Hint :
#  All files with the ".dat" ending will be taken into account
#  Don't use the .dat extension for the output file

###
### import libraries
###

import glob
import os
import getopt
import re
import commands

import datetime
import sys

from array import array
from numpy import *  # history: numeric ... numarray ... numpy
from optparse import OptionParser
from shutil import *

#function used to remove non-alpha-numeric characters
def clean(string):
	cleanedString = ""
	for i in string :
		if i.isalnum() or i == '.':
			cleanedString = cleanedString + i
	return cleanedString

# takes a float, tells if integer cast value is same value
def isInteger(value=0.0):
    if value == float(int(value)):
        return True
    else:
        return False

# define parser options
usage = "usage %prog [options]"
parser = OptionParser()
parser.add_option("-d","--directory",dest="dataDirectory",help="Search the .log file in DIRECTORY (default value = current directory). The script will recursively search with a depth of 2", metavar="DIRECTORY", default=".")
parser.add_option("-f","--file",dest="logFile",help="The log file that has to bee analyzed by the script (default value = none)", default="none")
parser.add_option("-b","--begin",dest="begin",help="Start to plot the curve from the FIRST evaluation (default value = 0)", metavar="FIRST", default="0")
parser.add_option("-e","--end",dest="end",help="Stop to plot the curve at the LAST evaluation (default value = 0). If the value 0 it will stop at the last evaluation of the log", metavar="LAST", default="0")

parser.add_option("-s","--step", dest="step",help="display a graph every STEP generation", metavar = "STEP", default="10" )

(options, args) = parser.parse_args()

if (not options.logFile == "none"):
	if os.path.isfile(options.logFile):
		filenames=[options.logFile]
	else:
		parser.error("The path ("+ str(options.logFile) + ") doesn't lead to any file")

else:
	# search for all files in current directory with ".log" extension
	path=options.dataDirectory + '/'
	filenames = glob.glob(os.path.join(path,'*.log'))
	temp = glob.glob(os.path.join(path,'*/*.log'))
	if len(temp) > 0:
		for name in temp:
			filenames.append(name)

# display general information

print '#'
print '# Sort the X best individuals from the logs of the Cortex board.'
print '# ' + str(datetime.datetime.now().ctime())
print '#'
print '# source(s):'
for filename in filenames:
    print '#\t' + filename + '\n',
print '#'
print '#'

# "define" data index in matches of the patterns
idEvEvaluation = 1
idEvRobot = 2
idEvEnergy = 3
idEvDeltaEnergy = 4
idEvGenomeListSize = 5

# load files -- all files with the ".dat" extension are loaded within fileContentArray (= an array of file contents)
for filename in filenames:
	print filename
	#	Initialization
	map={}
	fileContentArray = open(filename,'r').readlines()
	filePath = os.path.dirname(filename)

	print "load : " + str(datetime.datetime.now().ctime())

	for j in range(len(fileContentArray)):
		patternDescription = "#GIteration: ([0-9]+) #Robot: ([0-9]+) #Energy: (\-?[0-9]+\.?[0-9]*) #DeltaEnergy: (\-?[0-9]+\.?[0-9]*) #GenomeListSize: ([0-9]+)"
		patternEvaluation = re.compile(patternDescription)
		match = patternEvaluation.match(fileContentArray[j])
		if(match):
			if(not map.has_key(match.group(idEvEvaluation))):
				map[match.group(idEvEvaluation)] = {}
			if(not map[match.group(idEvEvaluation)].has_key(match.group(idEvRobot))):
				map[match.group(idEvEvaluation)][match.group(idEvRobot)] = {}
			map[match.group(idEvEvaluation)][match.group(idEvRobot)]['deltaEnergy'] = match.group(idEvDeltaEnergy)
			map[match.group(idEvEvaluation)][match.group(idEvRobot)]['genomeListSize'] = match.group(idEvGenomeListSize)


	maxIteration = 0
	if options.end == '0':
		for i in map.keys():
			if int(i) > maxIteration:
				maxIteration = int(i)
	else:
		maxIteration = int(options.end)


	for iteration in range(int(options.begin),maxIteration*600):
		if ( ( (float(iteration+1)/600) % float(options.step) ) == 0):
			print iteration
			print float(iteration+1)/600.0
			fileData = open("tmp.dat","w")
			for robot in map[str(iteration)]:
				fileData.write(map[str(iteration)][robot]['deltaEnergy'])
				fileData.write("," + map[str(iteration)][robot]['genomeListSize'])
				fileData.write("\n")
			fileData.close()
			title = str((iteration+1)/600) + ".eps"
			logPlotContent = 'plot '
			logPlotContent=logPlotContent+' \'tmp.dat\' using 1:2 with points notitle'
			copyfile('logplothead2d',filePath + '/logplot.gp')
			fileGnuplot = open(filePath + '/logplot.gp',"a")
			fileGnuplot.write('set output \'' + re.sub('\.log',title,filename) + '\'\n')
			fileGnuplot.write(logPlotContent)
			fileGnuplot.close()

			gnuPlotOutput = commands.getoutput("gnuplot " + filePath + "/logplot.gp")	
			print gnuPlotOutput




