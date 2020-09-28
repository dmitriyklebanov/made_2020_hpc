#!/bin/bash

my_array=(1 42 kokoko 24 2020 also very interesting text _for_ printing)

for i in "${my_array[@]}"
do
    echo "$i"
done