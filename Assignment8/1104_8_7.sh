#!/bin/bash

# Name: Atmik Goswami
# Roll: 002211001104
# Date: Mon Oct 28 12:29:13 IST 2024
# Assignment No: 8.7
# Assignment Details: Create a file that lists all of the .C files in your directory.
# Execution Command: sudo chmod +x 1104_8_7.sh
#                    ./1104_8_7.sh
# Output: List of .C files saved in C_files_list.txt:
#         ./test.c

# List all .C files and save them to a file named "C_files_list.txt".
find . -type f -name "*.c" > C_files_list.txt
echo "List of .C files saved in C_files_list.txt:"
cat C_files_list.txt
