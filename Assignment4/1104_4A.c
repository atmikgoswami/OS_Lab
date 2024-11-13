/**************************************************************************************************************************************
 * 
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Sep 23 13:09:43 IST 2024
 *      
 *  Assignment No: 4
 *  Assignment Details:
 *  Objective of this programming assignment is to use mmap() call and observe page-fault using the ‘sar’ command.
 *
 *  Input Description: None
 *  Output Description: 
 *  Compilation command: gcc 1104_4A.c -o Output4A
 *  Execution sequence: ./Output4A
 *  		        In other terminal: sar -B 1                   
 *                               
 *  Sample Input: None
 *  Sample Output:
 *  [be22104@localhost Assignment4]$ sar -B 1
 *  Linux 3.10.0-514.21.1.el7.x86_64 (localhost.localdomain)        Monday 23 September 2024        _x86_64_        (16 CPU)
 *
 *  02:41:04  IST  pgpgin/s pgpgout/s   fault/s  majflt/s  pgfree/s pgscank/s pgscand/s pgsteal/s    %vmeff
 *  02:41:05  IST      0.00      0.00    725.00      0.00    669.00      0.00      0.00      0.00      0.00
 *  02:41:06  IST      0.00      8.00    702.00      0.00    605.00      0.00      0.00      0.00      0.00
 *  02:41:07  IST      0.00      0.00    532.00      0.00    553.00      0.00      0.00      0.00      0.00
 *  02:41:08  IST      0.00      0.00    704.00      0.00    656.00      0.00      0.00      0.00      0.00
 *  02:41:09  IST      0.00      0.00    703.00      0.00    658.00      0.00      0.00      0.00      0.00
 *  02:41:10  IST     68.00    196.00    832.00      8.00    697.00      0.00      0.00      0.00      0.00
 *  02:41:11  IST    372.28   3754.46   1074.26    185.15    841.58      0.00      0.00      0.00      0.00
 *  02:41:12  IST     24.00   3536.00   1316.00    176.00    974.00      0.00      0.00      0.00      0.00
 *  02:41:13  IST     40.00   4008.00   1101.00    196.00   2080.00      0.00      0.00      0.00      0.00
 *  02:41:14  IST     20.00   3964.00    919.00    194.00    653.00      0.00      0.00      0.00      0.00
 *  02:41:15  IST     16.00   4028.00   1102.00    199.00    770.00      0.00      0.00      0.00      0.00
 *  02:41:16  IST      8.00   4136.00   1114.00    204.00    729.00      0.00      0.00      0.00      0.00
 *  02:41:17  IST      0.00   1200.00    828.00     60.00    668.00      0.00      0.00      0.00      0.00
 *  02:41:18  IST      4.00    324.00    736.00     16.00    659.00      0.00      0.00      0.00      0.00
 *  02:41:19  IST     24.00   3240.00   1029.00    162.00    701.00      0.00      0.00      0.00      0.00
 *  02:41:20  IST     16.00   3868.00   1092.00    193.00    670.00      0.00      0.00      0.00      0.00
 *  02:41:21  IST     16.00   3824.00   1073.00    191.00    575.00      0.00      0.00      0.00      0.00
 *  02:41:22  IST      4.00   3216.00   1116.00    159.00    849.00      0.00      0.00      0.00      0.00
 *  02:41:23  IST     20.00   4056.00   1115.00    205.00    683.00      0.00      0.00      0.00      0.00
 *  02:41:24  IST     12.00   3272.00   1037.00    165.00    664.00      0.00      0.00      0.00      0.00
 *  02:41:25  IST      4.00   3856.00   1091.00    192.00    680.00      0.00      0.00      0.00      0.00
 *  ^C
 *
 *  Average:          31.03   2404.76    949.64    119.32    763.59      0.00      0.00      0.00      0.00
 ***********************************************************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>

#define FILE_SIZE (8LL * 1024 * 1024 * 1024) // 8 GB

/* Function to handle errors and exit */
void error_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    /* Create a large file (8 GB) in /usr/tmp */
    const char *filename = "/usr/tmp/bigfile.dat"; 
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) error_exit("open");

    /* Allocate space for the file */
    if (fallocate(fd, 0, 0, FILE_SIZE) == -1) error_exit("fallocate");

    /* Map the file into memory */
    uint8_t *mapped = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) error_exit("mmap");

    /* Seed the random number generator */
    srand(time(NULL));

    while (1) {
        /* Generate random offset and value */
        off_t offset = rand() % FILE_SIZE;
        uint8_t value = rand() % 256;

        /* Write value to the specific offset */
        mapped[offset] = value;

        /* Read value back from the same offset */
        uint8_t read_value = mapped[offset];

        /* Print the offset in hex */
        printf("Offset: 0x%lx, Written Value: %d, Read Value: %d\n", (unsigned long)offset, value, read_value);

        /* Verify the read value */
        if (read_value != value) {
            fprintf(stderr, "Error: Value mismatch at offset 0x%lx\n", (unsigned long)offset);
            break; // Exit the loop on error
        }

        /* Sleep for a bit to reduce output frequency (optional) */
        usleep(10000); // Sleep for 10 milliseconds
    }

    /* Cleanup */
    munmap(mapped, FILE_SIZE);
    close(fd);
    return 0;
}

