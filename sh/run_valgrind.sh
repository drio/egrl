#!/bin/bash

cd tests
n=$1
if [ ".$n" == "." ];then
  n=10000
fi

head -$n reads.small.fq > r.txt
head -$n probes.small.txt > p.txt
valgrind --tool=memcheck --leak-check=yes \
  ../egrl count -p ./p.txt -r ./tests/r.txt | wc -l
rm -f r.txt p.txt
