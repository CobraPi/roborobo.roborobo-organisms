#!/usr/bin/awk -f

BEGIN {
	if (maxCount == null) {
		maxCount = 0
	}
	count = 0
	sum = 0;
	list = ""
	setting = ""
	total_count = 0
	total_avg = 0
}

{
	# remove the seed from the command
	utility = $1

	$1 = ""
	$2 = ""
	$3 = ""
	$4 = ""
	$5 = ""
	$(NF-1) = ""
	$NF = ""
		
	nwSetting = $0
	#nwSetting = $6 " " $7 " " $8 " " $9 " " $10 " " $11 " " $12 " " $13 " " $14 " " $15 " " $16 " " $17 " " $18 " " $19 " " $20 " " $21

	if (nwSetting != setting) {
		finish_setting()
		setting = nwSetting
	}
	if (maxCount == 0 || count < maxCount) {
		list = list " " utility
		sum = sum + utility
		count++
	}
}

END {
	finish_setting()
	#print "average is " total_avg " over " total_count " settings"
}

function finish_setting() {
	# determine the average for this parameter setting
	if (setting != "") {
		avg = sum / count
		print avg " (" list " ): " setting
		count = 0;
		sum = 0
		list = ""

		total_avg = ((total_avg * total_count) + avg) / (total_count + 1)
		total_count++
	}
}
