#!/bin/bash

mkdir 1-3
mkdir 2-4
mkdir 5
mkdir 6

cd 5
mkdir stake
mkdir stake-independent

cd ..


cp plot-1-3kout-64-1-0.500000-unif.csv 1-3/64.csv
cp plot-1-3kout-128-1-0.500000-unif.csv 1-3/128.csv
cp plot-1-3kout-256-1-0.500000-unif.csv 1-3/256.csv
cp plot-1-3kout-512-1-0.500000-unif.csv 1-3/512.csv
cp plot-1-3kout-1024-1-0.500000-unif.csv 1-3/1024.csv
cp plot-1-3kout-2048-1-0.500000-unif.csv 1-3/2048.csv
cp plot-1-3kout-4096-1-0.500000-unif.csv 1-3/4096.csv
cp plot-1-3kout-8192-1-0.500000-unif.csv 1-3/8192.csv


cp plot-2-4kout-1024-1-0.500000-exp.csv 2-4/1-exp.csv
cp plot-2-4kout-1024-1-0.500000-ff.csv 2-4/1-ff.csv
cp plot-2-4kout-1024-1-0.500000-unif.csv 2-4/1-unif.csv

cp plot-2-4kout-1024-2-0.500000-exp.csv 2-4/2-exp.csv
cp plot-2-4kout-1024-2-0.500000-ff.csv 2-4/2-ff.csv
cp plot-2-4kout-1024-2-0.500000-unif.csv 2-4/2-unif.csv

cp plot-2-4kout-1024-3-0.500000-exp.csv 2-4/3-exp.csv
cp plot-2-4kout-1024-3-0.500000-ff.csv 2-4/3-ff.csv
cp plot-2-4kout-1024-3-0.500000-unif.csv 2-4/3-unif.csv




cp plot-5kout-1024-3-0.500000-1.csv 5/stake/1.csv
cp plot-5kout-1024-3-0.500000-1k.csv 5/stake/1k.csv
cp plot-5kout-1024-3-0.500000-1m.csv 5/stake/1m.csv
cp plot-5kout-1024-3-0.500000-1b.csv 5/stake/1b.csv

cp plot-5kout-stake-independent-1024-3-0.500000-1.csv 5/stake-independent/1.csv
cp plot-5kout-stake-independent-1024-3-0.500000-1k.csv 5/stake-independent/1k.csv
cp plot-5kout-stake-independent-1024-3-0.500000-1m.csv 5/stake-independent/1m.csv
cp plot-5kout-stake-independent-1024-3-0.500000-1b.csv 5/stake-independent/1b.csv



cp plot-6-1024-0-0.000000-exp_1.csv 6/exp-1.csv
cp plot-6-1024-0-0.000000-exp_k.csv 6/exp-1k.csv
cp plot-6-1024-0-0.000000-exp_m.csv 6/exp-1m.csv
cp plot-6-1024-0-0.000000-exp_b.csv 6/exp-1b.csv
cp plot-6-1024-0-0.000000-ff.csv 6/ff.csv
cp plot-6-1024-0-0.000000-unif.csv 6/unif.csv
