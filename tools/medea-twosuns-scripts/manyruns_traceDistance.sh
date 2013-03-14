cp manyruns_traceDistance_template.gp tmp.many.d.gp
cat $1 | grep monitorDistance | sed 's/monitorDistance:\ //' | sort -n -t, -k3,3n -k4,4n > tmp.many.rawdata
python manyruns_aggregateValues.py >> tmp.many.d.gp
gnuplot tmp.many.d.gp
rm -f tmp.many.rawdata
rm -f tmp.many.d.gp
epspdf plot.manyruns.d.eps