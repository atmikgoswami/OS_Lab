#!/bin/bash

# Name: Atmik Goswami
# Roll: 002211001104
# Date: Mon Oct 28 12:29:13 IST 2024
# Assignment No: 8.10
# Assignment Details: Read ‘n’ and generate a pattern given below.
#                       1
#                       1 2
#                       1 2 3
#                       1 2 3 4
# Execution Command: sudo chmod +x 1104_8_10.sh
#                    ./1104_8_10.sh
# Output:
# [be22104@localhost Assignment8]$ ./1104_8_10.sh
# Enter the number of rows: 4
# 1
# 1 2
# 1 2 3
# 1 2 3 4

#Read the value of 'n' from the user.
read -p "Enter the number of rows: " n

#Loop to print the pattern.
for ((i = 1; i <= n; i++));do
  for ((j = 1; j <= i; j++));do
    echo -n "$j "
   done
  echo
done
