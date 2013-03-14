# aggregate data from multiple datalog files.
# used both for orientation and distance values
#
# Example:
#     transform:
#			1,1,0,0,1 #from datalog1
#			1,1,0,0,3 #from datalog2
#	  into:
#			1,1,0,0,4
#	...

import csv
import sys

a = -1
b = -1
c = -1
d = -1
e = -1 

sumOfDensities = 0

#cr = csv.reader(open("tmp.many.rawdata","rb"))
cr = csv.reader(open(sys.argv[1],"rb"))

# example:
#for row in cr:
#    print row
#    print row[2]


for row in cr:
	if ( a == -1 ):
		# init
		a = row[0]
		b = row[1]
		c = row[2]
		d = row[3]
		e = row[4]
		sumOfDensities = 0
	else:
		if ( row[3] != d ): # test if new group
			print str(a) + "," + str(b) + "," + str(c) + "," + str(d) + "," + str(sumOfDensities)
			sumOfDensities = 0
			a = row[0]
	                b = row[1]
                	c = row[2]
        	        d = row[3]
	                e = row[4]
		sumOfDensities = sumOfDensities + int(row[4])

print str(a) + "," + str(b) + "," + str(c) + "," + str(d) + "," + str(sumOfDensities)

