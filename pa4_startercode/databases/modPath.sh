#!/bin/bash

if [ -z $1 ] || [ -z $2 ]; then
    echo "Usage: ./scriptName inFile outFile"
    exit 2
fi

sed -r 's/\)--\[/\t/g' < $1 | sed -r 's/^\(//' | sed -r 's/\)$//' | sed -r 's/\]-->\(/\t/g' > $2

