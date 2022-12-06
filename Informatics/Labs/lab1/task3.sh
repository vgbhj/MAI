#!/bin/bash

read -a arr
len=${#arr[@]}

for(( i=1; i<len; ++i ))
do
    for(( j=1; j<len; ++j ))
    do
        if [ ${arr[j-1]} -gt ${arr[j]} ]
        then
            tmp_n=${arr[j-1]}
            arr[j-1]="${arr[j]}"
            arr[j]="$tmp_n"
        fi 
    done
done


if [ "$1" != "-r" ]
then
    echo ${arr[@]}
else
    for(( i=0; i<len; ++i ))
    do
        echo -n "${arr[len-i-1]} " 
    done
    echo
fi