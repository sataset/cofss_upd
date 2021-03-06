c = 299792.458
lambda = 1885.0
shift = c / lambda
#shift = 159.0410917772
pi = 3.1415926536

set term png font 'Fira Sans Reqular' 36 size 7680, 3810
set datafile separator ", "
set key top left invert

file_freq = "logs/freq_logs.csv"
file_time = "logs/time_logs.csv"

################################

stats file_time

N = STATS_blocks - 2
command = 'mkdir logs/plot_png'
system(command)

################################

do for [j=0:N] {
	set output 'logs/plot_png/cycle_' . j . '.png'
	set multiplot layout 2, 1 title 'Cycle ' . j

	set title 'Frequency domain'
	set xrange [1850:1920]
	set xlabel 'Wavelength [nm]'
	set ylabel 'Power [W]'

	plot \
	file_freq index j u (c / ($1/2/pi + shift)):2 w lines lw 3 lc 'orange' title 'Right Polarization' ,\
	file_freq index j u (c / ($1/2/pi + shift)):3 w lines lw 3 lc 'skyblue' title 'Left Polarization'

	set title 'Time domain'
	set xrange [*:*]
	set xlabel 'Time [ps]'
	set ylabel 'Power [W]'

	plot \
	file_time index j u 1:($2*$2 + $3*$3) w lines lw 4 lc 'orange' title 'Right Polarization' ,\
	file_time index j u 1:($4*$4 + $5*$5) w lines lw 4 lc 'skyblue' title 'Left Polarization'

	unset multiplot
}
