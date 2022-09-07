#!/bin/bash

if [[ $@ = "setup" ]]
then
    mkdir results
    cd results
    mkdir 1-3
    mkdir 2-4
    mkdir 5
    mkdir 6
elif [[ $@ = "clean-results" ]]
then
    rm -f results/1-3/*.csv
    rm -f results/2-4/*.csv
    rm -f results/5/*.csv
    rm -f results/6/*.csv
elif [[ $@ = "clean-all" ]]
then
    rm -rf results/
else
    echo "usage: $0 [setup|clean-results|clean-all]";
fi

