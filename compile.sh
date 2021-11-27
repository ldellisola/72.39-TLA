#!/bin/bash

if (($# != 1))
then
    echo 'Must enter INPUT input_path. Usage: ./compile.sh [input_path]'
    exit 1
fi
make compile INPUT=$1
