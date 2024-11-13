#!/bin/bash

# Name: Atmik Goswami
# Roll: 002211001104
# Date: Mon Oct 28 12:29:13 IST 2024
# Assignment No: 8.9
# Assignment Details: Write a script to convert string lower to upper and upper to lower from a file.
# Execution Command: sudo chmod +x 1104_8_9.sh
#                    ./1104_8_9.sh <filename>
# Output:
# [be22104@localhost Assignment8]$ cat test.txt
# hello WORLD
# [be22104@localhost Assignment8]$ ./1104_8_9.sh test.txt
# HELLO world

# Check if a file name was provided as an argument.
if [ -z "$1" ]; then
          echo "Usage: $0 filename"
            exit 1
fi

# Use 'tr' to toggle case and display the output.
tr 'a-zA-Z' 'A-Za-z' < "$1"
