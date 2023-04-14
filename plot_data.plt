unset key
unset colorbox
set cbrange [0:1]
set title 'data1'
set term png
set output "data1.png"
set palette defined ( 0 '#ff0000',\
                      1 '#09ad00')
plot 'data' using 3:4:2 with points lw 5 lc palette

set output "data2.png"
set title 'data2'

plot 'data2' using 3:4:2 with points lw 5 lc palette
