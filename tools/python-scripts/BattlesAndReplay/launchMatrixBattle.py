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

usage = "usage %prog [options] \n\
Remove the .on file in order to stop the program. The current update of the battle matrix will be cancelled"
parser = OptionParser()
parser.add_option("-d","--directory",dest="dataDirectory",help="Search the .log file in DIRECTORY (default value = current directory). The script will recursively search with a depth of 2", metavar="DIRECTORY", default=".")
parser.add_option("-f","--file",dest="logFile",help="The log file that has to bee analyzed by the script (default value = none)", default="none")
parser.add_option("-b","--begin",dest="begin",help="Start to place genomes in the battle from the FIRST generation (default value = 0)", metavar="FIRST", default="0")
parser.add_option("-e","--end",dest="end",help="Stop to place genomes in the battle at the LAST generation (default value = 0). If the value 0 it will fail", metavar="LAST", default="0")
parser.add_option("-s","--step",dest="step",help="Take individual randomely every STEP generation for the battle (default value = 10)", metavar="STEP", default="10")

parser.add_option("-l","--lifeTime",dest="lifeTime",help="life time of the robots in the considered experiments (default = 400)", metavar="LIFETIME", default="400")
parser.add_option("-o","--outputFile",dest="outputFile",help="The resulting battle matrix will be written in OUTPUTFILE.log. The resulting plot will be written in OUTPUTFILE.eps (default value = res)", metavar = "OUTPUTFILE", default="res")
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
	if (len(filenames) == 0):
		parser.error("There isn't any .log file at depth 2 in the specified directory")

if (int(options.end) == 0):
	parser.error("Please define the last generation")

# display general information

print '#'
print '# Produce a matrix of battle between the selected generations'
print '# ' + str(datetime.datetime.now().ctime())
print '#'
print '# source(s):'
for filename in filenames:
    print '#\t' + filename + '\n',
print '#'
print '#'

#Use the .on file
if os.path.isfile(".on"):
	print "It seems that a dual battle is already running. If you are sure it isn't the case, remove the \".on\"file present in this directory and try again."
	exit(1)
else:
	fileOn = open(".on","w")
	fileOn.close()

#If anly previous log file, back it up
if os.path.exists("./" + options.outputFile + ".log"):
	dataFileName = "./" + options.outputFile + ".log"
	dataFileNameBackup = dataFileName
	version = -1
	while ( os.path.exists(dataFileNameBackup) ):
		version += 1
		dataFileNameBackup = dataFileName + str(version)
	if ( not version == -1 ):
		move(dataFileName,dataFileNameBackup)
		print "The file \"" + dataFileName + "\" has been saved in \"" + dataFileNameBackup + "\""

#As long as you want to battle
nbOfUpdates = 0
deadFile = open("./deadFile","a")
while ( os.path.exists(".on") ):
	deadFile.write("\n")
	nbOfUpdates += 1
	#Create an empty matrixResultDiff
	matrixResultDiff = {}
	for generation1 in range(int(options.begin),int(options.end),int(options.step)):
		matrixResultDiff[generation1] = {}
		for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
			matrixResultDiff[generation1][generation2] = 0.0

	#Update the value of the matrix diff
	for iteration1 in range(int(options.begin)*int(options.lifeTime)-1,int(options.end)*int(options.lifeTime),int(options.lifeTime)*int(options.step)):
		generation1 = (iteration1+1)/int(options.lifeTime)
		for iteration2 in range(iteration1+(int(options.lifeTime)*int(options.step)),int(options.end)*int(options.lifeTime),int(options.lifeTime)*int(options.step)):
			generation2 = (iteration2+1)/int(options.lifeTime)
			print str(generation1) + "::" + str(generation2)
			totalRatio = 0.0
			nbFile = 0
			#Do the battle on each file
			for filename in filenames:
				nbFile += 1
				print "load log file : " + str(datetime.datetime.now().ctime())
				map={}
				fileContentArray = open(filename,'r').readlines()
				filePath = os.path.dirname(filename)
				#Fill the map with infos from the file
				for j in range(len(fileContentArray)):
					patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) use genome :(.*)"
					patternGenome = re.compile(patternDescription)
					genomeMatch = patternGenome.match(fileContentArray[j])
					if(genomeMatch):
						idIteration = genomeMatch.group(1)
						idBot = genomeMatch.group(2)
						genome = re.split(" ",genomeMatch.group(3))
						genome = genome[:-1]
						if(not map.has_key(str(int(idIteration)+int(options.lifeTime)))):
							map[str(int(idIteration)+int(options.lifeTime))] = {}
						if(not map[str(int(idIteration)+int(options.lifeTime))].has_key(idBot)):
							map[str(int(idIteration)+int(options.lifeTime))][idBot] = {}
						map[str(int(idIteration)+int(options.lifeTime))][idBot]['genome'] = genome

				selectedRobotList = {}
				#Pick some genome for the battle from iteration 1
				if map.has_key(str(iteration1)):
					choiceList = []
					for robot in map[str(iteration1)]:
						if ( map[str(iteration1)][robot].has_key("genome") ):
							choiceList.append(robot)
						else:
							print "I want the genome of robot : " +str(selectedRobot)
							exit()
					
					selectedRobot = choice(choiceList)
										
					selectedRobotList[generation1] = {}
					selectedRobotList[generation1]['genome'] = map[str(iteration1)][str(selectedRobot)]['genome']
					selectedRobotList[generation1]['generation'] = (iteration1+1)/int(options.lifeTime)
				else:
					print "It seems that there is no robot in this file at generation" + str(generation1)
					exit(1)

				#Pick some genome for the battle from iteration 2
				if map.has_key(str(iteration2)):
					choiceList = []
					for robot in map[str(iteration2)]:
						if ( map[str(iteration2)][robot].has_key("genome") ):
							choiceList.append(robot)
						else:
							print "I want the genome of robot : " +str(selectedRobot)
							exit()
					
					selectedRobot = choice(choiceList)
										
					selectedRobotList[generation2] = {}
					selectedRobotList[generation2]['genome'] = map[str(iteration2)][str(selectedRobot)]['genome']
					selectedRobotList[generation2]['generation'] = (iteration2+1)/int(options.lifeTime)
				else:
					print "It seems that there is no robot in this file at generation" + str(generation1)
					exit(1)

				#Create the properties file
				copyfile('battlehead','./config/battle.properties')
				battleFile = open('./config/battle.properties',"a")
				battleFile.write('gMaxIt=  ' + options.lengthBattle + '\n' )
				battleFile.write('gExperimentNumber=  ' + options.expSetup + '\n' )
				battleFile.write('nbGenomeToLoad= 2 \n' )
				#write the selected genomes in the properties file
				selectedCpt = 0
				if ( not len(selectedRobotList) == 2 ):
					print "Not enought robot in the battle"
					exit(1)
				for robot in selectedRobotList:
					geneCpt = 0
					for gene in selectedRobotList[robot]['genome']:
						battleFile.write('genomeToLoad'+str(selectedCpt)+'['+str(geneCpt)+'] = '+gene+'\n')
						geneCpt +=1
					selectedCpt += 1

				battleFile.close()

				#launch the battle
				print "launchBattle -- : " + str(datetime.datetime.now().ctime())

				battleOutput = commands.getoutput("./roborobo -l config/battle.properties")	
				print battleOutput

				#load the results of the battle
				print "load battle result : " + str(datetime.datetime.now().ctime())
				battleResult = open("logs/log.txt",'r').readlines()

				#if the user still want to run the battles, we will load the results and update matrixBattleDiff
				if (os.path.exists(".on")):
					#search the last genomes in the battle and find there percentage (in number of robots)
					finalGenome={}
					for j in range(len(battleResult)):
						patternDescription = "Info\(([0-9]+)\) : robot nb.([0-9]+) use genome :(.*)"
						patternGenome = re.compile(patternDescription)
						genomeMatch = patternGenome.match(battleResult[j])
						if(genomeMatch):
							idIteration = genomeMatch.group(1)
							idBot = genomeMatch.group(2)
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

					#update the values of totalGeneration1 and totalGeneration2
					generation1Counter = 0
					generation2Counter = 0
					for robot in genomePie:
						for robotComp in selectedRobotList :
							if ( selectedRobotList[robotComp]['genome'] == genomePie[robot]['genome'] ):
								if ( selectedRobotList[robotComp]['generation'] == generation1 ):
									generation1Counter += genomePie[robot]['cpt']
								if ( selectedRobotList[robotComp]['generation'] == generation2 ):
									generation2Counter += genomePie[robot]['cpt']
					del genomePie
					totalRatio += float(generation1Counter) / float(generation1Counter + generation2Counter)
					deadFile.write(str(100-generation1Counter+generation2Counter)+",")

				else:
					print "The .on file is no more there. The program will stop. Current update of the matrix is cancelled"
					break

				del selectedRobotList
			#end of loop on filename
			if (not os.path.exists(".on")):
				break
			else:
				#update the matrixResultDiff
				matrixResultDiff[generation1][generation2] = totalRatio/float(nbFile)
		#end of loop on iteration1
		deadFile.write("::")
		if (not os.path.exists(".on")):
			break
	#end of loop on iteration2
	deadFile.write("--")
	#apply the update 
	if (os.path.exists(".on")):
		#if the outputFile.log doesn't exist : create it
		if (not os.path.exists("./" + options.outputFile + ".log")):
			resFile = open("./" + options.outputFile + ".log","w")
			for generation1 in range(int(options.begin),int(options.end),int(options.step)):
				for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
					resFile.write(str(matrixResultDiff[generation1][generation2])+",")
			resFile.close()
			matrixResultDiff = {}
		#if the outputFile.log exist apply the update on it
		else:
			#load the existing file
			resFileContentArray = open("./"+options.outputFile+".log",'r').readlines()
			splitedContent = re.split(',',resFileContentArray[0])
			#apply the updates
			elementCurrentlyReaded = 0
			for generation1 in range(int(options.begin),int(options.end),int(options.step)):
				for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
					previousRatio = matrixResultDiff[generation1][generation2] 
					newRatio = float(splitedContent[elementCurrentlyReaded])
					matrixResultDiff[generation1][generation2] = ((previousRatio*(nbOfUpdates-1))+newRatio)/nbOfUpdates
					elementCurrentlyReaded += 1
				
			#re-write the updates
			resFile = open("./" + options.outputFile + ".log","w")
			for generation1 in range(int(options.begin),int(options.end),int(options.step)):
				for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
					resFile.write(str(matrixResultDiff[generation1][generation2])+",")
			resFile.close()
			matrixResultDiff = {}
#end of loop on .on file
deadFile.close()

#Print the results in gnuplot
#Create the .dat file
#Create an empty matrixResultTemp
matrixResultTemp = {}
for generation1 in range(int(options.begin),int(options.end),int(options.step)):
	matrixResultTemp[generation1] = {}
	for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
		matrixResultTemp[generation1][generation2] = 0

#fill the matrixResultTemp
resFileContentArray = open("./"+options.outputFile+".log",'r').readlines()
splitedContent = re.split(',',resFileContentArray[0])
elementCurrentlyReaded = 0
for generation1 in range(int(options.begin),int(options.end),int(options.step)):
	for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
		matrixResultTemp[generation1][generation2] += float(splitedContent[elementCurrentlyReaded])
		elementCurrentlyReaded += 1

#Rewrite nicely the matrixResultTemp
resFile = open("./tmp.dat","w")
for generation1 in range(int(options.begin),int(options.end)+int(options.step),int(options.step)):
	for generation2 in range(int(options.begin),generation1,int(options.step)):
		resFile.write(str(1-matrixResultTemp[generation2][generation1])+",")
	resFile.write("0,")
	for generation2 in range(generation1+int(options.step),int(options.end)+int(options.step),int(options.step)):
		resFile.write(str(matrixResultTemp[generation1][generation2])+",")
	resFile.write("\n")
resFile.close()

#create the logplot.gp
copyfile('logplotheadMatrix',filePath + '/logplot.gp')
fileGnuplot = open(filePath + '/logplot.gp',"a")
fileGnuplot.write('set output \'' + options.outputFile + '.eps\'\n')
fileGnuplot.write('splot \"tmp.dat\" matrix')
fileGnuplot.close()

#print the file
gnuPlotOutput = commands.getoutput("gnuplot " + filePath + "/logplot.gp")	
print gnuPlotOutput
