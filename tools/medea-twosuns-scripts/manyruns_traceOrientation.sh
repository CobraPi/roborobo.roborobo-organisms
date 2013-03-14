cp manyruns_traceOrientation_template.gp tmp.many.o.gp
cat $1 | grep monitorOrientation | sed 's/monitorOrientation:\ //' | sort -n -t, -k3,3n -k4,4n > tmp.many.rawdata
python manyruns_aggregateValues.py >> tmp.many.o.gp
gnuplot tmp.many.o.gp
rm -f tmp.many.rawdata
rm -f tmp.many.o.gp
epspdf plot.manyruns.o.eps
