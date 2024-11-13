#!/bin/bash

# Name: Atmik Goswami
# Roll: 002211001104
# Date: Mon Oct 28 12:29:13 IST 2024
# Assignment No: 8.8
# Assignment Details: Write a script for addition of two numbers for real numbers also.
# Execution Command: sudo chmod +x 1104_8_8.sh
#                    ./1104_8_8.sh
# Output:
# Enter the first number: 5
# Enter the second number: 6
# The sum is: 11

# Read two numbers, including real numbers, from the user.
read -p "Enter the first number: " num1
read -p "Enter the second number: " num2

# Calculate the sum using bc for precision with real numbers.
sum=$(echo "$num1 + $num2" | bc)
echo "The sum is: $sum"
