#!/bin/sh 
# usage     s-calc /usr/bin/clang

# pwd
# echo $1 $2
# ls .

./$2 < s-input.txt > s-output.s
$1 s-output.s
./a.out

