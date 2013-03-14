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

###
### define constant
###

xmin = -26
xmax = 13
ymin = -14
ymax = 27
size_square = 3


###
### define functions
###

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

# takes a list of value and print quartiles
def computeQuartiles(values=[],avg=0.0,stddev=0.0):
	# prepare list
	#values=[]
	#for item in dict:
	#	values.append(dict[item])
	values.sort()
	
	# compute stats
	medianIndex = (len(values) - 1.) / 2.
	lowerQuartileIndex = medianIndex / 2.
	upperQuartileIndex = medianIndex / 2. * 3.
	min    = values[0]
	max    = values[-1]
	
	if isInteger(medianIndex) is True:
		medianValue = values[int(medianIndex)]
	else:
		medianValue = ( values[int(medianIndex)] + values[int(medianIndex)+1] ) / 2.
	
	if isInteger(lowerQuartileIndex) is True:
		lowerQuartileValue = values[int(lowerQuartileIndex)]
	else:
		lowerQuartileValue = ( values[int(lowerQuartileIndex)] + values[int(lowerQuartileIndex)+1] ) / 2.
	
	if isInteger(upperQuartileIndex) is True:
		upperQuartileValue = values[int(upperQuartileIndex)]
	else:
		upperQuartileValue = ( values[int(upperQuartileIndex)] + values[int(upperQuartileIndex)+1] ) / 2.
	
	# display stats (whisker+box values)
	return [min,lowerQuartileValue,medianValue,upperQuartileValue,max]

###
### Setting up
###

# define parser options
usage = "usage %prog [options]"
parser = OptionParser()
parser.add_option("-d","--directory",dest="dataDirectory",help="Search the .log file in DIRECTORY (default value = current directory). The script will recursively search with a depth of 2", metavar="DIRECTORY", default=".")
parser.add_option("-b","--begin",dest="begin",help="Start to plot the curve from the FIRST evaluation (default value = 0)", metavar="FIRST", default="0")
parser.add_option("-e","--end",dest="end",help="Stop to plot the curve at the LAST evaluation (default value = 0). If the value 0 it will stop at the last evaluation of the log", metavar="LAST", default="0")
parser.add_option("-s","--step",dest="step",help="Plot a quartille at each step, given in a number of iteration (default Value = 10) ", metavar="STEP", default="10")
parser.add_option("-o","--output",dest="output",help="The name of the resulting plot (default=plot.eps)", metavar="OUTPUT", default="plot.eps")

parser.add_option("-L","--genomeList", action = "store_true", dest="meanGenomeListSize",help="Use this option in order to see the sizes of the genome lists in the robots of the swarm", default=False )
parser.add_option("-E","--deltaEnergy", action = "store_true", dest="meanDeltaEnergy",help="Use this option if you want to see the values of the delta energy", default=False )
parser.add_option("-S","--sigma", action = "store_true", dest="meanSigmaUsed",help="Use this option if you want to see the values of the sigma used", default=False )
parser.add_option("-A","--activeCount", action = "store_true", dest="activeCount",help="Use this option if you want to see the number of active robots", default=False )

(options, args) = parser.parse_args()
# search for all files in current directory with ".log" extension
path=options.dataDirectory + '/'
filenames = []
temp = glob.glob(os.path.join(path,'*/datalog*.txt')) # remove */ for current dir only
if len(temp) > 0:
	for name in temp:
		filenames.append(name)
		

# display general information

print '#'
print '# Display the size of the genome\'s list for the swarm of all the runs in quartille way'
print '# ' + str(datetime.datetime.now().ctime())
print '#'
print '# source(s):'
for filename in filenames:
    print '#\t' + filename + '\n',
print '#'
print '#'



# prepare data
# build a map to merge all the data from files
# format of a datum in the file :
# #evaluationIndex: 0\t#Robot: 1\t#Energy: 750.000 \t#DeltaEnergy:700.000
# #activeGenome: XX XXX XX XXX -XX XX X -XXX XX XXX XXX -XXX X XX XXX -XX XXX -XXX XXX

# "define" data index in matches of the patterns
idEvEvaluation = 1
idEvRobot = 2
idEvEnergy = 3
idEvDeltaEnergy = 4
idEvGenomeListSize = 5
####
idGendefEvaluation = 1
idGendefRobot = 2
####
idSigmaEvaluation = 1
idSigmaRobot = 2
idSigmaSigma = 3
####
idEnEvaluation = 1
idEnRobot=2
####
idHEnEvaluation = 1
idHEnRobot=2
####
idFallEvaluation = 1
idFallRobot = 2
####
idGenEvaluation = 1
idGenRobot = 2
idGenOtherRobot = 3
####
idNewEvaluation = 1
idNewRobot = 2

runsAverageListSize = {}
runsAverageDeltaEnergy = {}
runsAverageSigmaUsed = {}
runsActiveCount = {}
	
# load files -- all files with the ".dat" extension are loaded within fileContentArray (= an array of file contents)
for filename in filenames:
	print filename
	#	Initialization
	map={}
	globalMap = {}
	fileContentArray = open(filename,'r').readlines()

	for j in range(len(fileContentArray)):
		patternDescription = "#GIteration: ([0-9]+) #Robot: ([0-9]+) #Energy: (\-?[0-9]+\.?[0-9]*) #DeltaEnergy: (\-?[0-9]+\.?[0-9]*) #GenomeListSize: ([0-9]+)"
		patternEvaluation = re.compile(patternDescription)
		match = patternEvaluation.match(fileContentArray[j])
		if(match):
			if(not map.has_key(match.group(idEvEvaluation))):
				map[match.group(idEvEvaluation)] = {}
			if(not map[match.group(idEvEvaluation)].has_key(match.group(idEvRobot))):
				map[match.group(idEvEvaluation)][match.group(idEvRobot)] = {}
			if (options.meanGenomeListSize):
				map[match.group(idEvEvaluation)][match.group(idEvRobot)]['genomeListSize'] = match.group(idEvGenomeListSize)
			if (options.meanDeltaEnergy):
				map[match.group(idEvEvaluation)][match.group(idEvRobot)]['deltaEnergy'] = match.group(idEvDeltaEnergy)

		patternDescription = "Info\(([0-9]+)\) : active count is ([0-9]+)"
		patternEvaluation = re.compile(patternDescription)
		match = patternEvaluation.match(fileContentArray[j])
		if(match):
			if(not globalMap.has_key(match.group(1))):
				globalMap[match.group(1)] = {}
			globalMap[match.group(1)]['activeCount'] = match.group(2)

		patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) has mutate the genome with sigma : ([0-9]+\.[0-9]+)"
		patternSigmaUsed = re.compile(patternDescription)
		match = patternSigmaUsed.match(fileContentArray[j])
		if(match):
			idBot = match.group(idSigmaRobot)
			sigma = match.group(idSigmaSigma)
			iteration = match.group(idSigmaEvaluation)
			if(not map.has_key(iteration)):
				map[iteration] = {}
			if(not map[iteration].has_key(idBot)):
				map[iteration][idBot] = {}
			map[iteration][idBot]['sigmaValue'] = sigma

		patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) take the genome from the robot nb.([0-9]+)"
		patternGenomeUsed = re.compile(patternDescription)
		match = patternGenomeUsed.match(fileContentArray[j])
		if(match):
			idBot = match.group(idGenRobot)
			idOtherBot = str(int(match.group(idGenOtherRobot)))
			iteration = match.group(idGenEvaluation)
			if(not map.has_key(iteration)):
				map[iteration] = {}
			if(not map[iteration].has_key(idBot)):
				map[iteration][idBot] = {}
			map[iteration][idBot]['loadedGenome'] = idOtherBot

		patternDescription = "Info\(([0-9]+)\) : Human intervention on robot ([0-9]+) \(Energy\)"
		patternHEnergy = re.compile(patternDescription)
		match = patternHEnergy.match(fileContentArray[j])
		if(match):
			idBot = match.group(idHEnRobot)
			iteration = match.group(idHEnEvaluation)
			if(not map.has_key(iteration)):
				map[iteration] = {}
			if(not map[iteration].has_key(idBot)):
				map[iteration][idBot] = {}
			map[iteration][idBot]['HumanEnergy'] = 1

	maxIteration = 0
	if options.end == '0':
		for i in map.keys():
			if int(i) > maxIteration:
				maxIteration = int(i)
	else:
		maxIteration = int(options.end)*400

	meanGenomeListSize = 0.0
	genomeListSizeList = []

	meanDeltaEnergy = 0.0
	deltaList = []

	sigmaUsed = {}
	used = {}
	prevSigmaUsed = {}
	meanSigmaUsed = 0.0
	sigmaList = []
	activeCountList = []
	meanActiveCount = 0.0

	updateActiveCount = False
	updateMeanGenomeListSize = False
	updateDeltaEnergy = False
	updateSigmaUsed = False

	for iteration in range(int(options.begin),maxIteration+1):

		if (globalMap.has_key(str(iteration))):
			if(globalMap[str(iteration)].has_key('activeCount')):
				activeCountList.append(int(globalMap[str(iteration)]['activeCount']))
				updateActiveCount = True

		if ( map.has_key(str(iteration))):
			for robot in map[str(iteration)]:
				if(map[str(iteration)][robot].has_key('genomeListSize')):
					genomeListSizeList.append(int(map[str(iteration)][robot]['genomeListSize']))
					updateMeanGenomeListSize = True

				if(map[str(iteration)][robot].has_key('deltaEnergy')):
					deltaList.append((map[str(iteration)][robot]['deltaEnergy']))
					updateDeltaEnergy = True

				if ( sigmaUsed.has_key(robot) ):
					prevSigmaUsed[robot] = sigmaUsed[robot]

				if(map[str(iteration)][robot].has_key('loadedGenome')):
					used[robot] = int(map[str(iteration)][robot]['loadedGenome'])
					updateGenomeUsed = True

				if map[str(iteration)][robot].has_key('sigmaValue'):
					updateSigmaUsed = True
					sigmaUsed[robot] = float(map[str(iteration)][robot]['sigmaValue']) 

				for robot in used:
					if (prevSigmaUsed.has_key(str(used[robot]))):
						sigmaList.append(prevSigmaUsed[str(used[robot])])

		generation=float(iteration+1)/400.0
		if (generation%int(options.step) == 0):

			if (updateMeanGenomeListSize == True):
				cpt = 0
				meanGenomeListSize = 0.0
				for robot in genomeListSizeList:
					if ( robot > 0):
						meanGenomeListSize = meanGenomeListSize + robot
						cpt = cpt + 1
				if (cpt != 0):
					meanGenomeListSize = meanGenomeListSize/cpt
				else:
					meanGenomeListSize = 0
				genomeListSizeList = []

			if (updateDeltaEnergy == True ):
				cpt = 0
				meanDeltaEnergy = 0.0
				for robot in deltaList :
					meanDeltaEnergy = meanDeltaEnergy + float(robot)
					cpt += 1
				if (cpt != 0):
					meanDeltaEnergy = meanDeltaEnergy/cpt
				else:
					meanDeltaEnergy = 0
				deltaList = [] 

			if (updateSigmaUsed == True) :
				cpt = 0
				meanSigmaUsed = 0.0
				something = False
				for robot in sigmaList:
					meanSigmaUsed = meanSigmaUsed + float(robot)
					cpt = cpt + 1
				if (cpt != 0):
					meanSigmaUsed = meanSigmaUsed/cpt
				else:
					meanSigmaUsed = 0
				sigmaList = []

			if ( updateActiveCount == True) :
				cpt = 0
				meanActiveCount = 0.0
				for robot in activeCountList:
					meanActiveCount = meanActiveCount + float(robot)
					cpt = cpt + 1
				if (cpt != 0):
					meanActiveCount= meanActiveCount/cpt
				else:
					meanActiveCount = 0
				activeCountList = []

			if (not runsAverageListSize.has_key(generation)):
				runsAverageListSize[generation] = []
			if (not runsAverageDeltaEnergy.has_key(generation)):
				runsAverageDeltaEnergy[generation] = []
			if (not runsAverageSigmaUsed.has_key(generation)):
				runsAverageSigmaUsed[generation] = []
			if (options.meanGenomeListSize):
				runsAverageListSize[generation].append(meanGenomeListSize)
			if (options.meanDeltaEnergy):
				runsAverageDeltaEnergy[generation].append(meanDeltaEnergy)
			if (options.meanSigmaUsed):
				runsAverageSigmaUsed[generation].append(meanSigmaUsed)
			if (not runsActiveCount.has_key(generation)):
				runsActiveCount[generation] = []
			if (options.activeCount):
				runsActiveCount[generation].append(meanActiveCount)
			updateActiveCount = False
			updateMeanGenomeListSize = False
			updateDeltaEnergy = False
			updateSigmaUsed = False

fileData = open('tmp.dat',"w")

for generation in runsAverageListSize:
	if (options.meanGenomeListSize):
		quartilleGenomeListSize = computeQuartiles(runsAverageListSize[generation])
		fileData.write(str(generation) + "," + str(quartilleGenomeListSize[0]) + "," + str(quartilleGenomeListSize[1]) + "," + str(quartilleGenomeListSize[2]) + "," + str(quartilleGenomeListSize[3]) + "," + str(quartilleGenomeListSize[4]) + "\n") 
	if (options.meanDeltaEnergy):
		quartilleDelatEnergy = computeQuartiles(runsAverageDeltaEnergy[generation])
		fileData.write(str(generation) + "," + str(quartilleDelatEnergy[0]) + "," + str(quartilleDelatEnergy[1]) + "," + str(quartilleDelatEnergy[2]) + "," + str(quartilleDelatEnergy[3]) + "," + str(quartilleDelatEnergy[4]) + "\n") 
	if (options.meanSigmaUsed):
		quartilleSigmaUsed = computeQuartiles(runsAverageSigmaUsed[generation])
		fileData.write(str(generation) + "," + str(quartilleSigmaUsed[0]) + "," + str(quartilleSigmaUsed[1]) + "," + str(quartilleSigmaUsed[2]) + "," + str(quartilleSigmaUsed[3]) + "," + str(quartilleSigmaUsed[4]) + "\n") 

for generation in runsActiveCount:
	if (options.activeCount):
		quartilleActiveCount = computeQuartiles(runsActiveCount[generation])
		fileData.write(str(generation) + "," + str(quartilleActiveCount[0]) + "," + str(quartilleActiveCount[1]) + "," + str(quartilleActiveCount[2]) + "," + str(quartilleActiveCount[3]) + "," + str(quartilleActiveCount[4]) + "\n") 

fileData.close()


cpt=2
if (options.meanGenomeListSize):
	logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 1 linecolor 3 lw 1 notitle whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle'
if (options.meanDeltaEnergy):
	logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 1 linecolor 3 lw 1 notitle whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle'
if (options.meanSigmaUsed):
	logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 1 linecolor 3 lw 1 notitle whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle'
if (options.activeCount):
	logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 1 linecolor 3 lw 1 notitle whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle'

#TODO : the title is in logplothead. Change it so it match the infos in debugXX.properties
if (options.meanGenomeListSize):
	copyfile('logplotheadL',options.dataDirectory + '/logplot.gp')
if (options.meanDeltaEnergy):
	copyfile('logplotheadD',options.dataDirectory + '/logplot.gp')
if (options.meanSigmaUsed):
	copyfile('logplotheadS',options.dataDirectory + '/logplot.gp')
if (options.activeCount):
	copyfile('logplotheadA',options.dataDirectory + '/logplot.gp')
fileGnuplot = open(options.dataDirectory + '/logplot.gp',"a")
fileGnuplot.write('set output \'' + options.output  + '\'\n')
fileGnuplot.write(logPlotContent)
fileGnuplot.close()

outputGnuplot = commands.getoutput("gnuplot " + options.dataDirectory + "/logplot.gp")	
print outputGnuplot
