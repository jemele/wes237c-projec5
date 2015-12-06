set term canvas
set output "plot.html"
set multiplot;
set size 1,0.5
set origin 0.0,0.5;
set title "Input Signal";
plot "chirp.txt" using 1:2 with lines;
set origin 0.0,0.0;
set title "Output Signal"
plot "output.txt" using 1:2 with lines
unset multiplot

