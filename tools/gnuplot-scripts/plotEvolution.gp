# create a graph from an "exp.data" file
#
# syntaxe:  gnuplot plot.gp
#
# no_generation fitness_moyenne ecart_type fitness_meilleur
# an output file "exp.eps" is created

set xlabel 'Number of Evaluations (n)'
set ylabel '(see legend)'
set title 'Evolution of a single robot with a (1+1)-ES adapted for online-evolution'
#set key left top 
set key box 
set key below

set datafile separator "," 

set pointsize 1

set xrange [0:*]
#set xrange [0:1000]

set yrange [-10:+10] reverse
set y2range [-100:+100] noreverse
#set yrange [0:*] reverse
#set y2range [0:*] noreverse

set ytics 0,1
set ytics nomirror
set y2tics 0, 10000

plot 'cortex.dat' using 1:($5) title "evaluation counter" with lines axis x1y1, 'cortex.dat' using 1:(1/$6) title "new individual found" with points axis x1y1, 'cortex.dat' using 1:($2) title "best fitness ever" with lines axis x1y2,  'cortex.dat' using 1:($3) title "stored individual current fitness" with lines axis x1y2

pause 3
reread

# Decommenter ce qui suit pour generer un fichier EPS en sortie

#set term post eps "Times-Roman" 8
#set size 5./10., 3./7.
#set output 'exp.eps'
#replot
#set term X11

# OU ALORS: 
set term postscript eps enhanced monochrome
set output 'output.eps'
replot

pause -1 

