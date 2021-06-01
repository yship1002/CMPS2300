#!/bin/bash
for i in {1..10}
do
    echo Compiling and execution are automated!!
    gcc -o sexy p6.c
    ./sexy
done
