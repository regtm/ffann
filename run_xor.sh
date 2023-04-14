echo 'Building...'
make
echo 'Starting evolution xor...'
./task_xor
echo 'Plotting...'
gnuplot plot_xor.plt

 
