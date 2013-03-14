# 2009 02 23 - niko(at)lri.fr
# History:
# 	started from my own "depouille" script (2008 01 09 -- updated 2008 11 for CEC)
#
# Syntax: simply execute the script without any command line parameters
#
# From results provided on on-line evolution with Cortex board, display global statistics
# 
# assumes that:
# - all files that end with the ".dat" suffix are to be taken into account
#
# note that:
# - hint: dont use the ".dat" extension for the output file (avoid messing up when relaunching script -- remember, all "*.dat" are considered as input! ) :-)
# - in the same directory, you can find a gnuplot script that can be used to display the resulting compiled datafile (whisker-box display)
# 
# -n.

###
### import libraries
###

import glob
import os

#import sys
#import time
import datetime

from array import array
from numpy import *  # history: numeric ... numarray ... numpy

###
### define functions
###

# takes a float, tells if integer cast value is same value
def isInteger(value=0.0):
    if value == float(int(value)):
        return True
    else:
        return False


# takes a list of value and print quartiles

def printQuartiles(index,sigma,values=[],avg=0.0,stddev=0.0):
	
	# prepare list
	
	values.sort()
	
	# compute stats
	
	medianIndex = (len(values) - 1.) / 2.
	lowerQuartileIndex = medianIndex / 2.
	upperQuartileIndex = medianIndex / 2. * 3.
	
	#print str(lowerQuartileIndex) + ' ; ' + str(medianIndex) + ' ; ' + str(upperQuartileIndex)
	
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
	print str(index) + ',' + str(sigma) + ',' + str(min) + ',' + str(lowerQuartileValue) + ',' + str(medianValue) + ',' + str(upperQuartileValue) + ',' + str(max) + ',' + str(int(avg)) + ',' + str(int(stddev))


###
### Setting up (load and store data from all files in current directory with ".dat" extension)
###

path=r'./'
i = 0
filenames = glob.glob(os.path.join(path,'*.dat'))
fileContentArray = []

# display general information

print '#'
print '# extract quartiles information from strong causality study'
print '# ' + str(datetime.datetime.now().ctime()) # some options: datetime.datetime.now() ; datetime.datetime.now().ctime() ; datetime.datetime.now().strftime()
print '#'
print '# source(s):'
for filename in filenames:
	print '#  ' + filename,
print '#'
print '#'
print '# Note: when display quartile, the first element is an index, (ie. index no. of current display sequence - convenient for plotting the results), the last is sigma value'
print '#'

quartileIndex = 0 # quartile display index -- mandatory for using plotwhiskerbox.gp

# load files -- all files with the ".dat" extension are loaded within fileContentArray (= an array of file contents)
for filename in filenames:
    fileContentArray.append(open(filename, 'r').readlines())
    #print filename
    i = i + 1


dataPerFileContentArray = []
maxFileLength = 0
maxFileIndex = 0
nbOfFiles = len(filenames)
totalNumberOfLines = 0
totalNumberOfStoredGenomes = 0

# prepare data
# for each file stored in fileContentArray, build an array of entries (ie. lines) - dataPerFileContentArray is a two dimensions array (1st: files, 2nd: file lines)
# maxFileIndex = max index no. among files (ie. length wrt. number of lines)
i = 0
for i in range(nbOfFiles):
    allLinesInCurrentFile = ''.join(fileContentArray[i]).split('\n')
    dataPerFileContentArray.append(allLinesInCurrentFile);
    totalNumberOfLines = totalNumberOfLines + len(allLinesInCurrentFile)
    if len(allLinesInCurrentFile) > maxFileLength :
        maxFileLength = len(allLinesInCurrentFile)
        maxFileIndex = i
    #print str(i) + ': ' + dataPerFileContentArray[0][2].split(' ')[1] + ' , ' + dataPerFileContentArray[0][2].split(' ')[targetIndex] 


###
### Main loop.
### note: at this point, dataPerFileContentArray is a two dimensions array that contains all lines for each file.
###




fitnessValueList = []
index = 0

currentSigma = float(0)

# for all files.
for i in range(0,nbOfFiles):

	print '# File no.' + str(i) + ' '



	# for all lines in current file 
	for j in range(0,len(dataPerFileContentArray[i])):
	
		#print ' line ' + str(j)
	
		# prepare data
		
		#currentParsedLine =  (dataPerFileContentArray[i][j].split('\t')[0]).split(',')  # strip-off comment part of line (begin with a '\t')
		currentParsedLine =  dataPerFileContentArray[i][j].split(',')
		currentParsedLine[1]

		if currentSigma != float(currentParsedLine[2]) and len(fitnessValueList) != 0:
			printQuartiles (index, currentSigma, fitnessValueList )
			currentSigma = float(currentParsedLine[2])
			fitnessValueList = []
			index = index + 1

		fitnessValueList.append ( float(currentParsedLine[1]) )
		#fitnessValueList.append ( float(currentParsedLine[1]) )
		#print fitnessValueList
	
	#printQuartiles  (index, fitnessValueList )

print '# note: avoid leaving blank line at the end of the data file otw the python script might throuw an error (reading an empty line)'


print '#'
print '# end.'
print '#'
