set key top left
set xlabel "Number of processors"
set ylabel "Speedup"
set title "Speedup according to the number of processors"
set xtics (1,2,4,8,16,32)
plot "speedup_np_lprecision" using 1:2 title "N = 128" with linespoint, \
     "speedup_np_lprecision" using 1:3 title "N = 256" with linespoint, \
     "speedup_np_lprecision" using 1:4 title "N = 512" with linespoint, \
     "speedup_np_lprecision" using 1:5 title "N = 1024" with linespoint

