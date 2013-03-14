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
from random import choice

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

def computeQuartiles(thing={},avg=0.0,stddev=0.0):
	# prepare list
	values=[]
	if ( type(thing) == dict ):
		for item in thing:
			values.append(thing[item])
	else:
		values = thing

	if (len(values) == 0):
		values.append(0)
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

usage = "usage %prog [options]"
parser = OptionParser()
parser.add_option("-d","--directory",dest="dataDirectory",help="Search the .log file in DIRECTORY (default value = current directory). The script will recursively search with a depth of 2", metavar="DIRECTORY", default=".")
parser.add_option("-f","--file",dest="logFile",help="The log file that has to bee analyzed by the script (default value = none)", default="none")
parser.add_option("-b","--begin",dest="begin",help="Start to plot the curve from the FIRST evaluation (default value = 0)", metavar="FIRST", default="0")
parser.add_option("-e","--end",dest="end",help="Stop to plot the curve at the LAST evaluation (default value = 100). If the value 0 it will stop at the last evaluation of the log", metavar="LAST", default="100")
parser.add_option("-s","--step",dest="step",help="take individual randomely every STEP generation for the battle (default value = 10)", metavar="STEP", default="10")

parser.add_option("-l","--lifeTime",dest="lifeTime",help="life time of the robots in the considered experiments (default = 100)", metavar="LIFETIME", default="100")
parser.add_option("-n","--battleNumber",dest="battleNumber",help="NUMBER of battle that will be done on one file (default 1)", metavar="NUMBER", default="1")
parser.add_option("-o","--outputFile",dest="outputFile",help="The resulting eps file will be stored at OUTPUTFILE.eps, and the resulting log file will be store at OUTPUTFILE.log (default value = res)", metavar = "OUTPUTFILE", default="res")
parser.add_option("-r","--run", action = "store_true", dest="run",help="Should be true in order to run the battle. If false, use directly the OUTPUTFILE.log (default False). If false you have to provide the option --nbFiles.", default=False )
parser.add_option("","--nbFiles",dest="nbFiles",help="Number of filed used to generate the OUTPUTFILE.log file. This options has to be set if -r is false. This options won't be read if -r is true. (default value = 0)", metavar="NBFILES", default="0")
parser.add_option("-t","--lengthBattle",dest="lengthBattle",help="length of each battle in number of iteration (default 1000).", metavar="LENGTHBATTLE", default="1000")
parser.add_option("-x","--expSetup",dest="expSetup",help="experiment setup used for all the battles (default 0).", metavar="EXPERIENCE_BATTLE", default="0")

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

if ( ( options.run == False )  and ( options.nbFiles == "0" ) ):
	parser.error("Run is false. You have to define the number of file used to generate the OUTFILE.log file. Try -h option ...")

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
idGendefEvaluation = 1
idGendefRobot = 2

nbOfFileUsed = int(options.nbFiles)

if (options.run == True ):
	if os.path.exists("./" + options.outputFile + ".log"):
		os.remove("./" + options.outputFile + ".log")

	for filename in filenames:
		nbOfFileUsed += 1
		print filename
		#	Initialization
		map={}
		fileContentArray = open(filename,'r').readlines()
		filePath = os.path.dirname(filename)

		print "load log file : " + str(datetime.datetime.now().ctime())

		for j in range(len(fileContentArray)):
			patternDescription = "#GIteration: ([0-9]+) #Robot: ([0-9]+) #Energy: (\-?[0-9]+\.?[0-9]*) #DeltaEnergy: (\-?[0-9]+\.?[0-9]*) #GenomeListSize: ([0-9]+)"
			patternEvaluation = re.compile(patternDescription)
			match = patternEvaluation.match(fileContentArray[j])
			if(match):
				if(not map.has_key(match.group(idEvEvaluation))):
					map[match.group(idEvEvaluation)] = {}
				if(not map[match.group(idEvEvaluation)].has_key(match.group(idEvRobot))):
					map[match.group(idEvEvaluation)][match.group(idEvRobot)] = {}
				map[match.group(idEvEvaluation)][match.group(idEvRobot)]['deltaEnergy'] = int(float(match.group(idEvDeltaEnergy)))
				map[match.group(idEvEvaluation)][match.group(idEvRobot)]['genomeListSize'] = int(match.group(idEvGenomeListSize))

			patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) use genome :(.*)"
			patternGenome = re.compile(patternDescription)
			genomeMatch = patternGenome.match(fileContentArray[j])
			if(genomeMatch):
				idIteration = genomeMatch.group(idGendefEvaluation)
				idBot = genomeMatch.group(idGendefRobot)
				genome = re.split(" ",genomeMatch.group(3))
				genome = genome[:-1]
				if(not map.has_key(str(int(idIteration)+int(options.lifeTime)))):
					map[str(int(idIteration)+int(options.lifeTime))] = {}
				if(not map[str(int(idIteration)+int(options.lifeTime))].has_key(idBot)):
					map[str(int(idIteration)+int(options.lifeTime))][idBot] = {}
				map[str(int(idIteration)+int(options.lifeTime))][idBot]['genome'] = genome

		maxIteration = 0
		if options.end == '0':
			for i in map.keys():
				if int(i) > maxIteration:
					maxIteration = int(i)/int(options.lifeTime)
		else:
			maxIteration = int(options.end)

		resultList = {}
		
		for i in range (int(options.begin),maxIteration,int(options.step)):
			resultList[i] = []

		for battleId in range(int(options.battleNumber)):

			selectedRobotList = {}
			selectedCpt = 0
			copyfile('battlehead','./config/battle.properties')
			battleFile = open('./config/battle.properties',"a")
			battleFile.write('gMaxIt=  ' + options.lengthBattle + '\n' )
			battleFile.write('gExperimentNumber=  ' + options.expSetup + '\n' )
			inTheBattle = ""
			for iterationPass in range(int(options.begin)*int(options.lifeTime),maxIteration*int(options.lifeTime),int(options.step)*int(options.lifeTime)):
				iteration = iterationPass - 1
				if map.has_key(str(iteration)):
					inTheBattle += str(float(iteration+1)/int(options.lifeTime)) + " " 
					choiceList = []
					for robot in map[str(iteration)]:
						if ( map[str(iteration)][robot].has_key("genome") ):
							choiceList.append(robot)
					
					selectedRobot = choice(choiceList)

					if (not map[str(iteration)][selectedRobot].has_key('genome')):
						print "I want the genome of robot : " +str(selectedRobot)
						exit()
								
					selectedRobotList[selectedCpt] = {}
					selectedRobotList[selectedCpt]['genome'] = map[str(iteration)][str(selectedRobot)]['genome']
					selectedRobotList[selectedCpt]['deltaEnergy'] = map[str(iteration)][str(selectedRobot)]['deltaEnergy']
					selectedRobotList[selectedCpt]['genomeListSize'] = map[str(iteration)][str(selectedRobot)]['genomeListSize']
					selectedRobotList[selectedCpt]['generation'] = (iteration+1)/int(options.lifeTime)

					geneCpt = 0
					for gene in map[str(iteration)][str(selectedRobot)]['genome']:
						battleFile.write('genomeToLoad'+str(selectedCpt)+'['+str(geneCpt)+'] = '+gene+'\n')
						geneCpt +=1

					selectedCpt +=1

			battleFile.write('nbGenomeToLoad=  ' + str(selectedCpt) + '\n' )
			battleFile.close()
			print inTheBattle

			print "launchBattle -- " + filename + " --(" +str(battleId) + "/" + options.battleNumber  + ") : " + str(datetime.datetime.now().ctime())

			battleOutput = commands.getoutput("./roborobo -l config/battle.properties")	
			print battleOutput

			battleResult = open("logs/log.txt",'r').readlines()

			print "load battle result : " + str(datetime.datetime.now().ctime())
			finalGenome={}
			for j in range(len(battleResult)):
					patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) use genome :(.*)"
					patternGenome = re.compile(patternDescription)
					genomeMatch = patternGenome.match(battleResult[j])
					if(genomeMatch):
						idIteration = genomeMatch.group(idGendefEvaluation)
						idBot = genomeMatch.group(idGendefRobot)
						genome = re.split(" ",genomeMatch.group(3))
						genome = genome[:-1]
						if(not finalGenome.has_key(idBot)):
							finalGenome[idBot] = {}
						finalGenome[idBot] = genome

			genomePie={}
			for robot in finalGenome:
				alreadyHere = False
				for already in genomePie:
					if ( finalGenome[robot] == genomePie[already]['genome'] ):
						genomePie[already]['cpt'] += 1
						alreadyHere = True
				if ( alreadyHere == False ):
					genomePie[robot]={}
					genomePie[robot]['genome'] = finalGenome[robot]
					genomePie[robot]['cpt'] = 1

			resFile = open(options.outputFile + '.log',"a")
			for robot in genomePie:
				for robotComp in selectedRobotList :
					if ( selectedRobotList[robotComp]['genome'] == genomePie[robot]['genome'] ):
						resFile.write( "----------------------"+'\n')
						resFile.write( filename +'\n')
						resFile.write( "percentage : " + str(genomePie[robot]['cpt']) +'\n')
						resFile.write( "genome : " + str(genomePie[robot]['genome'])+'\n')
						resFile.write( "deltaEnergy : " + str(selectedRobotList[robotComp]['deltaEnergy'])+'\n')
						resFile.write( "genomeListSize : " + str(selectedRobotList[robotComp]['genomeListSize'])+'\n')
						resFile.write( "generation : " + str(selectedRobotList[robotComp]['generation'])+'\n')
						resultList[selectedRobotList[robotComp]['generation']].append(genomePie[robot]['cpt'])
			resFile.close()

			del genomePie
			del selectedRobotList

		print "print battle result of the file: " + str(datetime.datetime.now().ctime())

		#for generation in resultList:
		#	for toComplete in range (int(options.battleNumber) - len(resultList[generation])):
		#		resultList[generation].append(0)
		
		#for generation in completeResultList:
		#	for toComplete in range (int(options.battleNumber) - len(completeResultList[generation])):
		#		completeResultList[generation].append(0)

		fileData = open("tmp.dat","w")
		#for key in resultList :
			#quartile = computeQuartiles(resultList[key])
			#fileData.write(str(key) + "," + str(quartile[0]) + "," + str(quartile[1]) + "," + str(quartile[2]) + "," + str (quartile[3]) + "," + str(quartile[4])+ '\n')
		for key in range(int(options.begin),int(options.end),int(options.step)):
			keyToCheck = key 
			if resultList.has_key(keyToCheck):
				fileData.write(str(keyToCheck) + "," + str((float(len(resultList[keyToCheck]))/float(options.battleNumber))*100) + "\n")
			else:
				fileData.write(str(keyToCheck) + ",0\n")
		fileData.close()

		cpt = 2
		#logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 3 lw 2 title \'Distibution the battles winner among generations\' whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle,'
		logPlotContent = 'plot \'tmp.dat\' using 2:xtic(1) ti col ,'
		logPlotContent = logPlotContent[:-1]

		copyfile('logplotheadBa',filePath + '/logplot.gp')
		fileGnuplot = open(filePath + '/logplot.gp',"a")
		fileGnuplot.write('set output \'' + re.sub('\.log','Battle.eps',filename) + '\'\n')
		fileGnuplot.write(logPlotContent)
		fileGnuplot.close()

		gnuPlotOutput = commands.getoutput("gnuplot " + filePath + "/logplot.gp")	
		print gnuPlotOutput
		del resultList
		del map

completeResultList = {}

maxIteration = 0
if options.end == '0':
	for i in map.keys():
		if int(i) > maxIteration:
			maxIteration = int(i)
else:
	maxIteration = int(options.end)

for i in range (int(options.begin),maxIteration,int(options.step)):
	completeResultList[i] = []

resFile = open(options.outputFile + '.log',"r")
resContentArray = resFile.readlines()

for j in range(len(resContentArray)):
	patternDescription = "----------------------"
	patternEvaluation = re.compile(patternDescription)
	match = patternEvaluation.match(resContentArray[j])
	if (match):
		generation = 0
		percentage = 0

		generationDescription = "generation : ([0-9]+)"
		generationEvaluation = re.compile(generationDescription)
		generationMatch = generationEvaluation.match(resContentArray[j+6])
		if (generationMatch):
			generation = int(generationMatch.group(1))
		else:
			print "Error : There is no generation value in the line " + str(j+6) + " : " + resContentArrayp[j+6]
			exit()

		percentageDescription = "percentage : ([0-9]+)"
		percentageEvaluation = re.compile(percentageDescription)
		percentageMatch = percentageEvaluation.match(resContentArray[j+2])
		if (percentageMatch):
			percentage = percentageMatch.group(1)	
		else:
			print "Error : There is no percentage value in the line " + str(j+2) + " : " + resContentArrayp[j+2]
			exit()

		completeResultList[generation].append(percentage)

resFile.close()

print "print complete battle result : " + str(datetime.datetime.now().ctime())

fileData = open("tmp.dat","w")
#for key in completeResultList :
	#quartile = computeQuartiles(completeResultList[key])
	#fileData.write(str(key) + "," + str(quartile[0]) + "," + str(quartile[1]) + "," + str(quartile[2]) + "," + str (quartile[3]) + "," + str(quartile[4]) + '\n' )
for key in range(int(options.begin),int(options.end)+1,int(options.step)):
	keyToCheck = key 
	if completeResultList.has_key(keyToCheck):
		fileData.write(str(keyToCheck) + "," + str((float(len(completeResultList[keyToCheck]))/((float(options.battleNumber))*nbOfFileUsed))*100) + "\n")
	else:
		fileData.write(str(keyToCheck) + ",0\n")
fileData.close()

cpt = 2
#logPlotContent='plot \'tmp.dat\' using 1:' + str(cpt+1) + ':'+ str(cpt) + ':' + str(cpt+4) + ':' + str(cpt+3) +' with candlesticks lt 3 lw 2 title \'Distibution the battles winner among generations\' whiskerbars, \'\' using 1:' + str(cpt+2) + ':'+ str(cpt+2) + ':' + str(cpt+2) +':' + str(cpt+2) + ' with candlesticks lt -1 lw 2 notitle,'
logPlotContent = 'plot \'tmp.dat\' using 2:xtic(1) ti col title \'Average percentage of presence after a battle\','
logPlotContent = logPlotContent[:-1]

copyfile('logplotheadBa','./logplot.gp')
fileGnuplot = open('./logplot.gp',"a")
fileGnuplot.write('set output \'' + options.outputFile + '.eps\' \n')
fileGnuplot.write(logPlotContent)
fileGnuplot.close()

gnuPlotOutput = commands.getoutput("gnuplot " + "./logplot.gp")	
print gnuPlotOutput
