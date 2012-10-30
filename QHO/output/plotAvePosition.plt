#!/usr/local/bin/gnuplot -persist
set terminal png
set output 'AverageX.png'
set xlabel 'Bin Number'
set ylabel 'Average Position'
set title 'Average Position vs. Bin Number'
set grid
unset key
plot "./output.dat" using 1:2 with points linecolor rgb "green"


