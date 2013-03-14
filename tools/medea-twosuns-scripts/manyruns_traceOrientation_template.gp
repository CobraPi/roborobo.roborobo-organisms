# TEMPLATE for tracking agents orientations wrt. reference (ie. center of envt)
# This template is used through the traceOrientation.sh shell script

set terminal postscript color eps "Times-Roman" 24
#set terminal postscript color eps #enhanced
set output 'plot.manyruns.o.eps'

# Color runs from white to red
#set palette rgbformula 2,-7,-7
#set palette rgbformula -4,-7,-7 # blue-white
set palette rgbformula -4,-7,-7

#set cbrange [0:6]
#set cblabel "Score"
#unset cbtics

set xrange [-0.5:]
set yrange [-50:50]

set datafile separator ","

set ylabel "orientations"
set xlabel "#generations"

set ytics ("South" -50, "East" -25, "North" 0, "West" 25, "South" 50)

set title "agents orientation wrt. center of the environment"\
         ."\n(heat map)"

# plot supposed orientation of sun	# ASSUME SUN CHANGES EVERY 50 GENERATIONS
#if(x,n) = (x>n?25.0:-25.0)
#plot [0:200][-100:100] -f(sin(x/16),0), '-' using 3:($4-50):5 with image

# plot orientation of agents
plot '-' using 3:($4-50):5 with image
