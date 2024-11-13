#!/bin/bash

# Name: Atmik Goswami
# Roll: 002211001104
# Date: Mon Oct 28 12:29:13 IST 2024
# Assignment No: 8.1 to 8.6
# Assignment Details: 1.Create a directory ‘All My Cprogs Files’ in your home directory.
#                     2.Move all the C files to the new creating directory.
#                     3.Show the list of files in your current directory.
#                     4.Show Current working directory.
#                     5.Display date in the dd:mm:yy format
#                     6.Count the number of files in your home directory
#
# Execution Command: sudo chmod +x 1104_8_1_6.sh
#                    ./1104_8_9.sh
# Output:
# [be22104@localhost Assignment8]$ ./1104_8_1_6.sh
# Files in current directory:
# 1104_8_10.sh  1104_8_1_6.sh  1104_8_7.sh  1104_8_8.sh  1104_8_9.sh  All_My_Cprogs_Files  C_files_list.txt  test.c  test.txt
# Current working directory:
# /home/usr/student/ug/yr22/be22104/OS_Lab/Ass8
# Current date (dd:mm:yy):
# 28:10:24
# Number of files in home directory:
# 25107

# 1. Create a directory ‘All My Cprogs Files’ in your home directory.
mkdir -p "$HOME/OS_Lab/Assignment8/All_My_Cprogs_Files"

# 2. Copy all the C files to the newly created directory.
cp *.c "$HOME/OS_Lab/Assignment8/All_My_Cprogs_Files" 2>/dev/null

# 3. Show the list of files in your current directory.
echo "Files in current directory:"
ls

# 4. Show current working directory.
echo "Current working directory:"
pwd

# 5. Display date in the dd:mm:yy format.
echo "Current date (dd:mm:yy):"
date +"%d:%m:%y"

# 6. Count the number of files in your home directory.
echo "Number of files in home directory:"
find "$HOME" -type f | wc -l
