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
def computeQuartiles(dict={},avg=0.0,stddev=0.0):
	# prepare list
	values=[]
	for item in dict:
		values.append(dict[item])
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

#compute the distance between two vector of the same size
def euclidian_distance(vec1,vec2):
	dist = 0.0
	if (not len(vec1) == len(vec2) ):
		print ("Try to compute the distance between two vectors of different size")
		exit()
	else:
		for i in range(len(vec1)):
			dist = dist + pow(float(vec1[i])-float(vec2[i]),2)
		return sqrt(dist)		

#compute the coverage of a robot
def compute_coverage(file,i):
	map = {}
	cpt = 0
	log = commands.getstatusoutput("head -n "+str((i+1)*200)+" "+file+" | tail -n 200")
	log = log[1].split('\n')
	
	for i in range(int((xmax-xmin)/size_square)):
		map[i]={}
		for j in range (int((ymax-ymin)/size_square)):
			map[i][j]=0

	#print map
	for line in log:
		elements = line.split(':')
		#print elements[10]
		x = int((float(elements[10])-xmin)/size_square)
		y = int((float(elements[11])-ymin)/size_square)
		#print(str(x)+"::"+str(y))
		if (map[x][y] == 0):
			map[x][y] = 1
			cpt += 1
	
	return cpt

###
### Setting up
###

# define parser options
usage = "usage %prog [options]"
parser = OptionParser()
parser.add_option("-d","--directory",dest="dataDirectory",help="Search the .log file in DIRECTORY (default value = current directory). The script will recursively search with a depth of 2", metavar="DIRECTORY", default=".")
parser.add_option("-f","--file",dest="logFile",help="The log file that has to bee analyzed by the script (default value = none)", default="none")
parser.add_option("-b","--begin",dest="begin",help="Start to plot the curve from the FIRST evaluation (default value = 0)", metavar="FIRST", default="0")
parser.add_option("-e","--end",dest="end",help="Stop to plot the curve at the LAST evaluation (default value = 0). If the value 0 it will stop at the last evaluation of the log", metavar="LAST", default="0")


parser.add_option("-s","--bestSurvival", action = "store_true", dest="bestSurvival",help="Use this option in order to see how many times the best genome has been used in other robots.", default=False )
parser.add_option("-n","--meanEnergy", action = "store_true", dest="meanEnergy",help="Use this option in order to see the mean energy of the swarm", default=False )
parser.add_option("-a","--meanDeltaEnergy", action = "store_true", dest="meanDeltaEnergy",help="Use this option if you want to see the mean value of the delta energy", default=False )
parser.add_option("-c","--coverage", action = "store_true", dest="coverage",help="Use this option in order to see the coverage rate of the swarm. To use this option you need to have a log of the positions of the robot in a logxx.txt file (where XX is the id of the robot)", default=False )
parser.add_option("-l","--meanGenomeListSize", action = "store_true", dest="meanGenomeListSize",help="Use this option in order to see mean size of the genome lists in the robots of the swarm", default=False )
parser.add_option("-G","--genDistFromAncestral", action = "store_true", dest="genDistFromAncestral",help="Show the genotypic distance between : the average agent in the swarm, and the average agent of the swarm at the beginning of the experiment", default=False )
parser.add_option("-g","--meanGenotypicDistance", action = "store_true", dest="meanGenotypicDistance",help="Use this option in order to see the mean euclidian distance between the genomes of the swarm", default=False )
parser.add_option("-p","--genDistFromPrev", action = "store_true", dest="genDistFromPrev",help="Show the genotypic distance between : the average agent in the swarm, and the average agent in the swarm at the previous evaluation", default=False )
parser.add_option("-t","--ticEnergy", action = "store_true", dest="ticEnergy",help="Use this option if you want to see a tic each time a robot has get an energy point", default=False )
parser.add_option("-i","--humanEnergyTic", action = "store_true", dest="humanEnergyTic",help="Use this option if you want to see a tic each time a human has give energy to the robot", default=False )
parser.add_option("-r","--restart", action = "store_true", dest="restart",help="Use this option if you want to see a tic each time a robot has try a whole new genome because there was no genome in its list", default=False )
parser.add_option("-m","--meanSigma", action = "store_true", dest="meanSigma",help="Use this option in order to see the sigma used to generate the genomes currently used by the swarm", default=False )
parser.add_option("-u","--restartDisplay", action = "store_true", dest="restartDisplay",help="This option will display a message on the standart output showing the last iteration when a restart occures in the poplulation", default=False )

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

	
# load files -- all files with the ".dat" extension are loaded within fileContentArray (= an array of file contents)
for filename in filenames:
	print filename
	#	Initialization
	map={}
	fileContentArray = open(filename,'r').readlines()
	filePath = os.path.dirname(filename)

	print "load : " + str(datetime.datetime.now().ctime())

	for j in range(len(fileContentArray)):
		if ( (options.meanEnergy) or (options.meanDeltaEnergy) or (options.meanGenomeListSize) ):
			patternDescription = "#GIteration: ([0-9]+) #Robot: ([0-9]+) #Energy: (\-?[0-9]+\.?[0-9]*) #DeltaEnergy: (\-?[0-9]+\.?[0-9]*) #GenomeListSize: ([0-9]+)"
			patternEvaluation = re.compile(patternDescription)
			match = patternEvaluation.match(fileContentArray[j])
			if(match):
				if(not map.has_key(match.group(idEvEvaluation))):
					map[match.group(idEvEvaluation)] = {}
				if(not map[match.group(idEvEvaluation)].has_key(match.group(idEvRobot))):
					map[match.group(idEvEvaluation)][match.group(idEvRobot)] = {}
				if (options.meanEnergy):
					map[match.group(idEvEvaluation)][match.group(idEvRobot)]['energy'] = match.group(idEvEnergy)
				if (options.meanDeltaEnergy):
					map[match.group(idEvEvaluation)][match.group(idEvRobot)]['deltaEnergy'] = match.group(idEvDeltaEnergy)
				if (options.meanGenomeListSize):
					map[match.group(idEvEvaluation)][match.group(idEvRobot)]['genomeListSize'] = match.group(idEvGenomeListSize)
	
		#TODO : use the new displaying of the genome
			#find the genome that correspond to the evaluationIndex
		if ( (options.genDistFromAncestral) or (options.meanGenotypicDistance) or (options.genDistFromPrev) or (options.meanSigma)):
			patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) use genome :(\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+) (\-?[0-9]+\.[0-9]+)"
			patternGenome = re.compile(patternDescription)
			genomeMatch = patternGenome.match(fileContentArray[j])
			if(genomeMatch):
				idIteration = genomeMatch.group(idGendefEvaluation)
				idBot = genomeMatch.group(idGendefRobot)
				genome = []
				for k in range(idGendefRobot+1,idGendefRobot+1+22) :
					genome.append(genomeMatch.group(k))
				if ( (options.genDistFromAncestral) or (options.meanGenotypicDistance) or (options.genDistFromPrev) or (options.meanSigma)):
					map[idIteration][idBot]['genome'] = genome

		if (options.ticEnergy):
			patternDescription = "Info\(([0-9]+)\) : ([0-9]+)\(.*\) get an energy point at .*"
			patternGetEnergy = re.compile(patternDescription)
			match = patternGetEnergy.match(fileContentArray[j])
			if(match):
				idBot = match.group(idEnRobot)
				iteration = match.group(idEnEvaluation)
				if(not map.has_key(iteration)):
					map[iteration] = {}
				if(not map[iteration].has_key(idBot)):
					map[iteration][idBot] = {}
				if(not map[iteration][idBot].has_key('getEnergy')):
					if (options.ticEnergy):
						map[iteration][idBot]['getEnergy'] = 1
				else:
					if (options.ticEnergy):
						map[iteration][idBot]['getEnergy'] = map[iteration][idBot]['getEnergy']+1

		if (options.meanSigma):
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
				if (options.meanSigma):
					map[iteration][idBot]['sigmaValue'] = sigma

		if (options.meanSigma):
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
				if (options.meanSigma):
					map[iteration][idBot]['loadedGenome'] = idOtherBot

		if (options.restart ):
			patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) is trying a whole new genome"
			patternNewGenome = re.compile(patternDescription)
			match = patternNewGenome.match(fileContentArray[j])
			if(match):
				idBot = match.group(idNewRobot)
				iteration = match.group(idNewEvaluation)
				if(not map.has_key(iteration)):
					map[iteration] = {}
				if(not map[iteration].has_key(idBot)):
					map[iteration][idBot] = {}
				if (options.restart ):
					map[iteration][idBot]['newGenome'] = 1

		if ( (options.humanEnergyTic) or (options.restartDisplay)):
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
				if ( (options.humanEnergyTic) or (options.restartDisplay)):
					map[iteration][idBot]['HumanEnergy'] = 1

		#patternDescription = "Info\(([0-9]+)\) : Human intervention on robot ([0-9]+) \(Fall\)"
		#patternFall = re.compile(patternDescription)
		#match = patternFall.match(fileContentArray[j])
		#if(match):
	#		idBot = match.group(idFallRobot)
#			iteration = match.group(idFallEvaluation)
#			if(not map.has_key(iteration)):
#				map[iteration] = {}
#			if(not map[iteration].has_key(idBot)):
#				map[iteration][idBot] = {}
			#map[iteration][idBot]['HumanFall'] = 1

	#for i in map:
	#	print(i)
	#	print(map[i])
	#	print ("####")

	fileData = open("tmp.dat","w")

	maxIteration = 0
	if options.end == '0':
		for i in map.keys():
			if int(i) > maxIteration:
				maxIteration = int(i)
	else:
		maxIteration = int(options.end)

	meanEnergy = 0.0
	energiesList = {}
	coverage = []
	meanGenomeListSize = 0.0
	genomeListSizeList = {}
	minDeltaEnergy = 0.0
	meanDeltaEnergy = 0.0
	maxDeltaEnergy = 0.0
	deltaList = {}
	used = {}
	genomeList = {}
	meanGenotypicDistance = 0.0
	genDistFromAncestral = 0.0
	firstGen = False
	nbGenes = -1
	ancestralGen = []
	genDistFromPrev = 0.0
	listPrev = []
	prevGen = []
	newPrevGen = []
	sigmaUsed = {}
	prevSigmaUsed = {}
	meanSigmaUsed = 0.0
	iteration = 0
	lastRestart = 0

	print "compute : " + str(datetime.datetime.now().ctime())

	for iteration in range(int(options.begin),maxIteration+1):
		energyCpt = 0
		humanEnergyCpt = 0
		restartCpt = 0
		if map.has_key(str(iteration)):
			updateMeanGenomeListSize = False
			updateEnergy = False
			updateDeltaEnergy = False
			updateGenomeUsed = False
			updateGenotypicDistance = False
			updateSigma = False
			#TODO : check use of synchronised before trying to find the best robot

			for robot in map[str(iteration)]:
				if(map[str(iteration)][robot].has_key('energy')):
					energiesList[robot] = float(map[str(iteration)][robot]['energy'])
					updateEnergy = True

				if(map[str(iteration)][robot].has_key('deltaEnergy')):
					deltaList[robot] = float(map[str(iteration)][robot]['deltaEnergy'])
					updateDeltaEnergy = True
			
				if(map[str(iteration)][robot].has_key('genomeListSize')):
					genomeListSizeList[robot] = int(map[str(iteration)][robot]['genomeListSize'])
					updateMeanGenomeListSize = True

				if ( sigmaUsed.has_key(robot) ):
					prevSigmaUsed[robot] = sigmaUsed[robot]
		
				if(map[str(iteration)][robot].has_key('loadedGenome')):
					used[robot] = int(map[str(iteration)][robot]['loadedGenome'])
					updateGenomeUsed = True

					if (not map[str(iteration)][robot].has_key('genome')):
						print "The value of the genome should be given when the genome is loaded. Analyze of" + filename + " :: timestep:" + str(iteration) + "; robot:" + robot
						exit()

					if (not map[str(iteration)][robot].has_key('sigmaValue')):
						print "The value of the sigma used to generate the genome should be given when the genome is loaded. Analyze of" + filename + " :: timestep:" + str(iteration) + "; robot:" + robot
						exit()

				if map[str(iteration)][robot].has_key('sigmaValue'):
					updateSigma = True
					sigmaUsed[robot] = float(map[str(iteration)][robot]['sigmaValue']) 

				if map[str(iteration)][robot].has_key('genome'):
					updateGenotypicDistance = True
					genomeList[robot] = map[str(iteration)][robot]['genome']

				if(map[str(iteration)][robot].has_key('getEnergy')):
					energyCpt = energyCpt + 1	

				if(map[str(iteration)][robot].has_key('HumanEnergy')):
					humanEnergyCpt = humanEnergyCpt + 1
					lastRestart = iteration

				if(map[str(iteration)][robot].has_key('newGenome')):
					restartCpt = restartCpt + 1


			#TODO : check in debug file if we are  in a synchronized case
			# for cover and bestGenome
			#TODO : write bestRobot and cover ....

			if ((options.meanEnergy == True) and (updateEnergy == True)):
				cpt = 0
				meanEnergy = 0.0
				for robot in energiesList :
					meanEnergy = meanEnergy + energiesList[robot]
					cpt = cpt +1
				meanEnergy = meanEnergy/cpt

			if ((options.meanDeltaEnergy == True) and (updateDeltaEnergy == True) ):
				cpt = 0
				maxDeltaEnergy = -100
				minDeltaEnergy = -100
				meanDeltaEnergy = 0.0
				for robot in deltaList :
					if ((minDeltaEnergy == -100) or (deltaList[robot] < minDeltaEnergy) ):
						minDeltaEnergy = deltaList[robot]
					if ((maxDeltaEnergy == -100) or (deltaList[robot] > maxDeltaEnergy) ):
						maxDeltaEnergy = deltaList[robot]
					meanDeltaEnergy = meanDeltaEnergy + deltaList[robot]
					cpt += 1
				meanDeltaEnergy = meanDeltaEnergy / cpt

			if ((options.meanGenomeListSize == True) and (updateMeanGenomeListSize == True) ):
				cpt = 0
				meanGenomeListSize = 0.0
				for robot in genomeListSizeList:
					meanGenomeListSize = meanGenomeListSize + genomeListSizeList[robot]
					cpt = cpt + 1
				meanGenomeListSize = meanGenomeListSize/cpt

			if ((options.meanGenotypicDistance == True) and (updateGenotypicDistance == True) ):
				cpt = 0
				minIdRobot = len(genomeList) + 1 
				maxIdRobot = -1
				for robot in genomeList:
					if int(robot) > maxIdRobot:
						maxIdRobot = int(robot)
					if int(robot) < minIdRobot:
						minIdRobot = int(robot)
				meanGenotypicDistance = 0.0
				for robot1 in range(minIdRobot, maxIdRobot):
					for robot2 in range (robot1+1, maxIdRobot+1):
						meanGenotypicDistance = meanGenotypicDistance + euclidian_distance(genomeList[str(robot1)],genomeList[str(robot2)])
						cpt = cpt + 1
				meanGenotypicDistance = meanGenotypicDistance / cpt

			if ((options.genDistFromAncestral == True) and (updateGenotypicDistance == True) ):
				if (firstGen == False):
					cptRob = 0
					for robot in genomeList:
						if (nbGenes == -1):
							nbGenes = len(genomeList[robot])
							ancestralGen = [0]
							ancestralGen = ancestralGen * nbGenes

						for geneId in range(0,nbGenes):
							ancestralGen[geneId] += float(genomeList[robot][geneId])
						cptRob += 1

					for geneId in range(0,nbGenes):
						ancestralGen[geneId] = ancestralGen[geneId] / cptRob
						
					firstGen = True

				else:
					newGen = [0]
					newGen = newGen * nbGenes
					cptRob = 0
					for robot in genomeList:
						for geneId in range(0,nbGenes):
							newGen[geneId] += float(genomeList[robot][geneId])
						cptRob += 1
					for geneId in range(0,nbGenes):
						newGen[geneId] = newGen[geneId] / cptRob

					genDistFromAncestral  = euclidian_distance(newGen,ancestralGen)

			if ((options.genDistFromPrev == True ) and (updateGenotypicDistance == True)):
				#if (len(newPrevGen) == nbGenes): 
				#	prevGen = newPrevGen
				#cptRob = 0
				#if (not nbGenes == -1):
				#	newPrevGen = [0]
				#	newPrevGen = newPrevGen * nbGenes
				#for robot in genomeList:
				#	if (nbGenes == -1):
				#		nbGenes = len(genomeList[robot])
				#		newPrevGen = [0]
				#		newPrevGen = newPrevGen * nbGenes
				#	for geneId in range(0,nbGenes):
				#		newPrevGen[geneId] += float(genomeList[robot][geneId])
				#	cptRob += 1
				#for geneId in range(0,nbGenes):
				#	newPrevGen[geneId] = newPrevGen[geneId] / cptRob
				
				#if (len(prevGen) == nbGenes):
				#	genDistFromPrev  = euclidian_distance(newPrevGen,prevGen)
				if (len(newPrevGen) == nbGenes): 
					if (not len(listPrev) == 50):
						listPrev.insert(0,newPrevGen)
					else:
						listPrev.pop()
						listPrev.insert(0,newPrevGen)

				cptRob = 0
				if (not nbGenes == -1):
					newPrevGen = [0]
					newPrevGen = newPrevGen * nbGenes
				for robot in genomeList:
					if (nbGenes == -1):
						nbGenes = len(genomeList[robot])
						newPrevGen = [0]
						newPrevGen = newPrevGen * nbGenes
					for geneId in range(0,nbGenes):
						newPrevGen[geneId] += float(genomeList[robot][geneId])
					cptRob += 1
				for geneId in range(0,nbGenes):
					newPrevGen[geneId] = newPrevGen[geneId] / cptRob
			
				if (len(listPrev) == 50):
					genDistFromPrev  = euclidian_distance(newPrevGen,listPrev[49])
				

						

			if ( (options.meanSigma == True)	and (updateSigma == True) ):
				meanSigmaUsed = 0.0
				cpt = 0
				for robot in used:
					if (prevSigmaUsed.has_key(str(used[robot]))):
						meanSigmaUsed = prevSigmaUsed[str(used[robot])] + meanSigmaUsed
						cpt += 1
				if ( not cpt == 0):
					meanSigmaUsed = meanSigmaUsed / cpt
					

			if(options.coverage):
				#meanCoverage = 0
				#for j in coverage:
				#	meanCoverage += j
				#meanCoverage /= len(coverage)
				file = filePath+"/log_c"+(str(int(j)+1))+".txt"
				if(os.path.isfile(file)):
					coverage.append(compute_coverage(file,i))
				else:
					print ("Log file for the robot " + j+1 + " is missing")
					exit()
				#else:
				#	coverage.append(0)

			fileData.write(str(iteration/100)) 
			if (options.meanEnergy == True):
				fileData.write("," + str(meanEnergy))
			if (options.meanDeltaEnergy == True):
				fileData.write(","+ str(meanDeltaEnergy) + "," + str(minDeltaEnergy) + "," + str(maxDeltaEnergy))
			if (options.meanGenomeListSize == True):
				fileData.write("," + str(meanGenomeListSize))
			if (options.meanGenotypicDistance == True):
				fileData.write("," + str(meanGenotypicDistance))
			if (options.genDistFromAncestral == True):
				fileData.write("," + str(genDistFromAncestral))
			if (options.genDistFromPrev == True):
				fileData.write("," + str(genDistFromPrev*5))
			if (options.ticEnergy == True):
				fileData.write("," + str(energyCpt))
			if (options.humanEnergyTic == True):
				fileData.write("," + str(humanEnergyCpt))
			if (options.restart == True):
				fileData.write("," + str(restartCpt))
			if (options.meanSigma == True):
				fileData.write("," + str(meanSigmaUsed*100))
			fileData.write("\n")

	
	fileData.close()

	if (options.restartDisplay):
		print lastRestart

	print filePath + " : " + str(datetime.datetime.now().ctime())

	logPlotContent = 'plot '
	cpt = 2
	if (options.meanEnergy == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"avg. energy in the swarm\" with lines,'
		cpt = cpt+1
	if (options.meanDeltaEnergy == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"avg. value of delta energy\" with lines,'
		#cpt = cpt+1
		#logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"min. value of delta energy\" with lines,'
		#cpt = cpt+1
		#logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"max. value of delta energy\" with lines,'
		#cpt = cpt+1
		cpt = cpt+3
	if (options.meanGenomeListSize == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"avg. nb of genome in list\" with lines,'
		cpt = cpt+1
	if (options.meanGenotypicDistance == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"avg. distance between the genomes of the swarm\" with lines,'
		cpt = cpt+1
	if (options.genDistFromAncestral == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"genotypic distance from the ancestral population\" with lines,'
		cpt = cpt+1
	if (options.genDistFromPrev == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"genotypic distance from the previous population \" with lines,'
		cpt = cpt+1
	if (options.ticEnergy == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:($' + str(cpt) + ' != 0 ? $'+ str(cpt)+' : 0) title \"nb of energy point taken\" with impulses,'
		cpt = cpt+1
	if (options.humanEnergyTic == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:($' + str(cpt) + ' != 0 ? $'+ str(cpt)+' : 0) title \"nb of time the robot was fed\" with impulses,'
		cpt = cpt+1
	if (options.restart == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:($' + str(cpt) + ' != 0 ? $'+ str(cpt)+' : 0) title \"nb of time the robot has restart\" with impulses,'
		cpt = cpt+1
	if (options.meanSigma == True):
		logPlotContent=logPlotContent+' \'tmp.dat\' using 1:' + str(cpt) + ' title \"avg. value of the sigma used to generate the genomes used by the swarm\" with lines,'
		cpt = cpt+1
	logPlotContent = logPlotContent[:-1]
	
	#TODO : the title is in logplothead. Change it so it match the infos in debugXX.properties
	copyfile('logplothead',filePath + '/logplot.gp')
	fileGnuplot = open(filePath + '/logplot.gp',"a")
	fileGnuplot.write('set output \'' + re.sub('\.log','.eps',filename) + '\'\n')
	fileGnuplot.write(logPlotContent)
	fileGnuplot.close()

	gnuPlotOutput = commands.getoutput("gnuplot " + filePath + "/logplot.gp")	
	print gnuPlotOutput
