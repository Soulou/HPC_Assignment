set xlabel 'x'
set ylabel 'y'
set key top
set xrange [0:1]
set yrange [0:1]
set zrange [0:1]
set samples 25
set isosamples 20
set pm3d
set palette model RGB
set palette rgbformulae 7,5,15
splot 'laplace_results.txt' with points lt -1
