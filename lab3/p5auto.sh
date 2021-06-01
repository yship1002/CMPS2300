#!/bin/bash
for i in {1..10}
do
    gcc -o sexy p5.c
    ./sexy
done
