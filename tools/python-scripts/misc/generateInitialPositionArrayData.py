# init pos


print '{'
for y in range(15):
	for x in range(15):
		for orient in range(4):
			print '  {' + str(x*50+50) + ' , ' + str( y*50+50 ) + ' , ' + str(orient*90) + ' } ,'
print '  {-1 , -1 , -1 },'
print '}'