# TEMPLATE for tracking agents distance to reference (ie. center of envt)
# This template is used through the traceDistance.sh shell script

set terminal postscript color eps "Times-Roman" 24
#set terminal postscript color eps #enhanced
set output 'plot.manyruns.d.eps'

# Color runs from white to red
#set palette rgbformula 2,-7,-7
#set palette rgbformula -4,-7,-7 # blue-white
set palette rgbformula -4,-7,-7

#set cbrange [0:6]
#set cblabel "Score"
#unset cbtics

set xrange [-0.5:]
set yrange [0:100]

set datafile separator ","

set ylabel "normalized distances"
set xlabel "#generations"

set title "agents distance to center of the environment"\
         ."\n(heat map)"

plot '-' using 3:4:5 with image
