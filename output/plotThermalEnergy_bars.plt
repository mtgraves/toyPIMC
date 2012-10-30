#!/usr/local/bin/gnuplot -persist
set terminal png
set output 'ThermEnergy_withBars.png'
set xlabel 'Bin Number'
set ylabel 'Thermodynamic Energy'
set title 'Thermodynamic Energy vs. Bin Number'
set grid
unset key
plot "./output.dat" using 1:4:5 with yerrorbars linecolor rgb "yellow"


