#!/opt/local/bin/gawk -f

{
	for (i=3; i<NF; i++) {
		if ($i == "):") {
			i=NF;
		} else {
			printf "0%s,", $i;
		}
	}
	print ""

	# print only the scored utilities, separated by commas
	# print "0" $3 ",0" $4 ",0" $5 ",0" $6 ",0" $7 ",0" $8 ",0" $9 ",0" $10 ",0" $11 ",0" $12 
}