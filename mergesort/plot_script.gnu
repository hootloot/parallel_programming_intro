set terminal png size 1200,780
set output 'mergesort_performance.png
set title 'Merge Sort Performance'
set xlabel 'Array size'
set ylabel 'Time (seconds)'
set grid
set style line 1 lc rgb 'blue' pt 7 ps 1.5 lt 1 lw 2
plot 'mergesort_data.txt' with linespoints ls 1 title 'Merge Sort'
