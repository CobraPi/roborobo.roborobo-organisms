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
def computeQuartiles(dict=[],avg=0.0,stddev=0.0):
	# prepare list
	values=dict
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


parser.add_option("-s","--step", dest="step",help="", default="25" )

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

	for j in range(len(fileContentArray)):
		patternDescription = "#GIteration: ([0-9]+) #Robot: ([0-9]+) #Energy: (\-?[0-9]+\.?[0-9]*) #DeltaEnergy: (\-?[0-9]+\.?[0-9]*) #GenomeListSize: ([0-9]+)"
		patternEvaluation = re.compile(patternDescription)
		match = patternEvaluation.match(fileContentArray[j])
		if(match):
			if(not map.has_key(match.group(idEvEvaluation))):
				map[match.group(idEvEvaluation)] = {}
			if(not map[match.group(idEvEvaluation)].has_key(match.group(idEvRobot))):
				map[match.group(idEvEvaluation)][match.group(idEvRobot)] = {}
			map[match.group(idEvEvaluation)][match.group(idEvRobot)]['genomeListSize'] = match.group(idEvGenomeListSize)

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

	deltaList = []
	deltaQuartille = [0,0,0,0,0]
	iterationCpt = 0

	for iteration in range(int(options.begin),maxIteration+1):
		energyCpt = 0
		humanEnergyCpt = 0
		restartCpt = 0
		if map.has_key(str(iteration)):
			updateDeltaEnergy = False

			for robot in map[str(iteration)]:
				if(map[str(iteration)][robot].has_key('genomeListSize')):
					deltaList.append(float(map[str(iteration)][robot]['genomeListSize']))
					updateDeltaEnergy = True
				if(map[str(iteration)][robot].has_key('HumanEnergy')):
					humanEnergyCpt = humanEnergyCpt + 1
					lastRestart = iteration

			if ( ( float(iteration+1)/400.0 ) % float(options.step) == 0 ):
				deltaQuartille = computeQuartiles(deltaList)
				fileData.write(str(iteration/400)) 
				fileData.write(","+ str(deltaQuartille[0]) +","+ str(deltaQuartille[1]) +","+ str(deltaQuartille[2]) +","+ str(deltaQuartille[3]) +","+ str(deltaQuartille[4])+ "," )# + str(-humanEnergyCpt) )
				fileData.write("\n")
				deltaList=[]
			#else:
			#	fileData.write(str(iteration/100)) 
			#	fileData.write(","+ str(-200) +","+ str(-200) +","+ str(-200) +","+ str(-200) +","+ str(-200) + "," + str(-humanEnergyCpt) )
			#	fileData.write("\n")

	fileData.close()
	print filePath

	logPlotContent = 'plot '
	cpt = 2
	logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 3 lw 2 title \'Quartiles of the size of the genome pool\' whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle,'
	cpt += 5
	#logPlotContent=logPlotContent+' \'tmp.dat\' using 1:($' + str(cpt) + ' != 0 ? $'+ str(cpt)+' : 0) title \"nb of restarts\" with impulses linetype 1,'
	logPlotContent = logPlotContent[:-1]
	
	#TODO : the title is in logplothead. Change it so it match the infos in debugXX.properties
	copyfile('logplotheadLQ',filePath + '/logplot.gp')
	fileGnuplot = open(filePath + '/logplot.gp',"a")
	fileGnuplot.write('set output \'' + re.sub('\.log','LQ.eps',filename) + '\'\n')
	fileGnuplot.write(logPlotContent)
	fileGnuplot.close()

	gnuPlotOutput = commands.getoutput("gnuplot " + filePath + "/logplot.gp")	
	print gnuPlotOutput
