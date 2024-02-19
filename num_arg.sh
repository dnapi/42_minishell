#!/bin/bash

#The argument counter
echo -n "The total number of arguments are: "
echo $#

# Loop through all arguments and print them separated by a newline
for arg in "$@"; do
    echo "$arg"
done
