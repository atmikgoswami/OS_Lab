/**************************************************************************************************************************************
 * 
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Oct 28 12:21:32 IST 2024
 *      
 *  Assignment No: 6
 *  Assignment Details:
 *  Write a C program which should print the following values on terminal:
 *      Processor Informations (Vendor ID, Model Name and Cache Size in MB)
 *      Kernel Name, Kernel Release and Operating System Name.
 *      The amount of memory configured into this computer and Free Memory
 *      Amount of time since the system was last booted (In Hours, Minutes, Seconds)
 *
 *  Input Description: None
 *  Output Description: 
 *  Compilation command: gcc 1104_6A.c -o Output6A
 *  Execution sequence: ./Output6A              
 *                               
 *  Sample Input: None
 *  Sample Output:
 *  [be22104@localhost Assignment6]$ ./Output6A
 *  Processor Vendor ID: GenuineIntel
 *  Processor Model Name: Intel(R) Xeon(R) CPU E5-2609 v4 @ 1.70GHz
 *  Cache Size: 20480 KB
 *  Kernel Info: Red Hat 4.8.5-11 3.10.0-514.21.1.el7.x86_64 mockbuild@sl7-uefisign.fnal.gov
 *  Total Memory: 64013.00 MB
 *  Free Memory: 60897.58 MB
 *  System Uptime: 557 hours, 52 minutes, 36 seconds
 ***********************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

void print_processor_info() {
        /* Open /proc/cpuinfo to read processor details */
        FILE *fp = fopen("/proc/cpuinfo", "r");
        if (fp == NULL) {
                perror("Error opening /proc/cpuinfo");
                return;
        }

        char line[256];
        char vendor_id[256];
        char model_name[256];
        int cache_size = 0;

        /* Read each line in /proc/cpuinfo */
        while (fgets(line, sizeof(line), fp)) {
                /* Check for vendor ID */
                if (sscanf(line, "vendor_id : %s", vendor_id) == 1) {
                        printf("Processor Vendor ID: %s\n", vendor_id);
                        /* Check for model name */
                } else if (sscanf(line, "model name : %[^\n]", model_name) == 1) {
                        printf("Processor Model Name: %s\n", model_name);
                        /* Check for cache size */
                } else if (sscanf(line, "cache size : %d", &cache_size) == 1) {
                        printf("Cache Size: %d MB\n", cache_size / 1024);  /* Convert KB to MB */
                }

                if (vendor_id[0] != '\0' && model_name[0] != '\0' && cache_size != 0) {
                        break;
                }
        }

        fclose(fp);  /* Close the file */
}

void print_kernel_info() {
        /* Open /proc/version to read kernel information */
        FILE *fp = fopen("/proc/version", "r");
        if (fp == NULL) {
                perror("Error opening /proc/version");
                return;
        }

        char line[256];
        fgets(line, sizeof(line), fp);
        printf("Kernel Info: %s", line);  /* Print kernel info */

        fclose(fp);  /* Close the file */
}

void print_memory_info() {
        /* Open /proc/meminfo to read memory statistics */
        FILE *fp = fopen("/proc/meminfo", "r");
        if (fp == NULL) {
                perror("Error opening /proc/meminfo");
                return;
        }

        char line[256];
        long total_memory = 0, free_memory = 0;
        /* Read each line in /proc/meminfo */
        while (fgets(line, sizeof(line), fp)) {
                /* Check for total memory */
                if (sscanf(line, "MemTotal: %ld kB", &total_memory) == 1) {
                        printf("Total Memory: %ld MB\n", total_memory / 1024);  /* Convert kB to MB */
                        /* Check for free memory */
                } else if (sscanf(line, "MemFree: %ld kB", &free_memory) == 1) {
                        printf("Free Memory: %ld MB\n", free_memory / 1024);    /* Convert kB to MB */
                }
        }

        fclose(fp);  /* Close the file */
}

void print_uptime() {
        /* Open /proc/uptime to read system uptime */
        FILE *fp = fopen("/proc/uptime", "r");
        if (fp == NULL) {
                perror("Error opening /proc/uptime");
                return;
        }
        double uptime_seconds;
        fscanf(fp, "%lf", &uptime_seconds);  /* Read uptime in seconds */
        fclose(fp);  /* Close the file */

        /* Convert uptime to hours, minutes, and seconds */
        int hours = uptime_seconds / 3600;
        int minutes = (uptime_seconds - (hours * 3600)) / 60;
        int seconds = (int)uptime_seconds % 60;

        printf("System Uptime: %d hours, %d minutes, %d seconds\n", hours, minutes, seconds);
}

int main() {
        /* Call functions to print system information */
        print_processor_info();
        print_kernel_info();
        print_memory_info();
        print_uptime();

        return 0;  /* Exit the program */
}
