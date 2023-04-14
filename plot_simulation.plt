unset key
set term png
set output "simulation.png"
plot 'map' lc "orange", 'simulation_result_data' using ($1):($2)
