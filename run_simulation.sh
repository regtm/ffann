echo 'Building...'
make
echo 'Starting evolution simulation task...'
./task_simulate_robot
echo 'Plotting...'
gnuplot plot_simulation.plt 
