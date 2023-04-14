set title 'Evolve XOR'
unset key
set term png
set output "classification_xor.png"
set hidden3d
set dgrid3d 50,50 qnorm 2
set xlabel 'A'
set ylabel 'B'
set zlabel 'Classification' rotate parallel
splot 'xor_classification' with lines 
