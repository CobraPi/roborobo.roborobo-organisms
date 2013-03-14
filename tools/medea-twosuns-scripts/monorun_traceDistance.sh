cp monorun_traceDistance_template.gp tmp.d.gp
cat $1 | grep monitorDistance | sed 's/monitorDistance:\ //' >> tmp.d.gp
gnuplot tmp.d.gp
rm -f tmp.d.gp
epspdf plot.monorun.d.pdf