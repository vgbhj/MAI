#!/bin/bash

function traverse() {   
    for file in $(ls "$1")
    do
        if [[ ! -d ${1}/${file} ]]; then
          # echo " ${file} is a file"
          if [[ ${file} == Makefile* ]]; then
            sed -i 's//g' ${1}/"$file";
          fi
        else
            # echo "entering recursion with: ${1}/${file}"
            traverse "${1}/${file}"
        fi
    done
}

function main() {
    traverse "$1"
}

main "$1"