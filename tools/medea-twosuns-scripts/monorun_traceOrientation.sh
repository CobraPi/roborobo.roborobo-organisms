cp monorun_traceOrientation_template.gp tmp.o.gp
cat $1 | grep monitorOrientation | sed 's/monitorOrientation:\ //' >> tmp.o.gp
gnuplot tmp.o.gp
rm -f tmp.o.gp
epspdf plot.monorun.o.eps