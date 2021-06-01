#!/bin/bash
for i in {1..10}
do
    gcc -o sexy p4.c
    ./sexy
done
