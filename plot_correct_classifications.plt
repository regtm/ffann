unset key
unset colorbox
set cbrange [0:1]
set title 'Correct classifications data1'
set term png
set output "classification_data1.png"
set palette defined ( 0 '#ff0000',\
                      1 '#09ad00')
plot 'classification_results_data1' using 1:2:(($3 == $4) ? 1 : 0) with points lw 5 lc palette
