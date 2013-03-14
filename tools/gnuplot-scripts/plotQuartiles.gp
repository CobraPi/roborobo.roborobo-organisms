# nicolas / 20090223
# restarted from plotWhiskerBox (depouille2/niko)
# source: http://www.gnuplot.info/demo/candlesticks.html
# box-and-whisker plot adding median value as bar

#set terminal png 
#set output 'results.png'

set boxwidth 0.8 absolute

#set title "box-and-whisker plot adding median value as bar" 

set datafile separator ","

#set log y

#set title "median, quartiles and average, standard deviation"
set title "strong causality from random genome"

#set xrange [ 0.0 : 4 ] noreverse nowriteback
#set yrange [ 1 : 50000.0 ] noreverse nowriteback
#set yrange [ 0. : 1000.0 ]
set xrange [ 0.0 : 17 ] noreverse nowriteback

# le every 200 donne le nombre de ligne a sauter.
plot 'results.log' every 1 using 1:4:3:7:5 with candlesticks lt 3 lw 2 title 'Quartiles' , '' using 1:4:4:4:4 with candlesticks lt -1 lw 2 notitle
#, '' every 1 using (0.5+$1):7 title 'Average' with dots, '' using (0.5+$1):7:8 title '' with errorbars 3


# notes on the experiment format file
# nbEval, min, min_quartile, median, max_quartile, max
# 1       1.5     2       2.4     4       6.
# 2       1.5     3       3.5     4       5.

# notes on results compiled from cmaes
# - file used is compiled from N experiments cmaes file with respect to previous format
# - nbEval is considered (and not iterations) - nbEval is not reset after restart
# - ALL DATA IN RESULT FILE ARE BASED ON BEST VALUES (or bestever -- but not median of each run -- take it as blackbox) 

set term postscript eps enhanced monochrome
set output 'results.eps'
replot

