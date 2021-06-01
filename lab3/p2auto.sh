#!/bin/bash
for i in {1..100}
do
    echo Compiling and execution are automated!!!!!
    gcc -o cfile p2.c
    ./cfile book.txt
done
