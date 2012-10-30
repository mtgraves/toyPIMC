#!/usr/local/bin/gnuplot -persist
set terminal png
set output 'ThermEnergy.png'
set xlabel 'Bin Number'
set ylabel 'Thermodynamic Energy'
set title 'Thermodynamic Energy vs. Bin Number'
set grid
unset key
plot "./output.dat" using 1:4 with points linecolor rgb "pink"


