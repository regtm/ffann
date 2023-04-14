echo 'Building...'
make
echo 'Starting evolution data1...'
./task_classification_data1
echo 'Plotting...'
gnuplot plot_data.plt
gnuplot plot_correct_classifications.plt

echo 'Starting evolution data2...'
./task_classification_data2
gnuplot plot_correct_classifications2.plt
