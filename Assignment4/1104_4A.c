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
 *  02:41:05  IST      0.00    318.00    539.00      0.00    224.00      0.00      0.00      0.00      0.00
 *  02:41:06  IST      0.00      0.00    844.00      0.00    320.00      0.00      0.00      0.00      0.00
 *  02:41:07  IST      0.00      0.00    559.00      0.00    327.00      0.00      0.00      0.00      0.00
 *  02:41:08  IST      0.00      0.00   1400.00      0.00    478.00      0.00      0.00      0.00      0.00
 *  02:41:09  IST      0.00      0.00    536.00      0.00    220.00      0.00      0.00      0.00      0.00
 *  02:41:10  IST      0.00    264.00    817.00      0.00   1007.00      0.00      0.00      0.00      0.00
 *  02:41:11  IST      0.00      0.00    934.00      0.00    343.00      0.00      0.00      0.00      0.00
 *  02:41:12  IST      0.00      0.00    531.00      0.00    221.00      0.00      0.00      0.00      0.00
 *  02:41:13  IST      0.00    256.00    781.00     30.00   7322.00      0.00      0.00      0.00      0.00
 *  02:41:14  IST      0.00      0.00    948.00     98.00    350.00      0.00      0.00      0.00      0.00
 *  02:41:15  IST      0.00      8.00    635.00     99.00    216.00      0.00      0.00      0.00      0.00
 *  02:41:16  IST      0.00      0.00   1031.00     98.00    330.00      0.00      0.00      0.00      0.00
 *  02:41:17  IST      0.00      0.00   1484.00     99.00    604.00      0.00      0.00      0.00      0.00
 *  02:41:18  IST      0.00      0.00    629.00     97.00    210.00      0.00      0.00      0.00      0.00
 *  02:41:19  IST      0.00      0.00   1032.00     98.00    331.00      0.00      0.00      0.00      0.00
 *  02:41:20  IST      0.00      0.00    627.00     98.00    213.00      0.00      0.00      0.00      0.00
 *  02:41:21  IST      0.00      0.00    625.00     97.00    212.00      0.00      0.00      0.00      0.00
 *  02:41:22  IST      0.00      0.00    631.00     97.00    225.00      0.00      0.00      0.00      0.00
 *  02:41:23  IST      0.00   1280.00    630.00     97.00    354.00      0.00      0.00      0.00      0.00
 *  02:41:24  IST      0.00   2432.00   1046.00     99.00    661.00      0.00      0.00      0.00      0.00
 *  02:41:25  IST      0.00    456.00    626.00     97.00    355.00      0.00      0.00      0.00      0.00
 *  02:41:26  IST      0.00      0.00   2931.00     98.00   1048.00      0.00      0.00      0.00      0.00
 *  02:41:27  IST      0.00      0.00    654.00     96.00    327.00      0.00      0.00      0.00      0.00
 *  ^C
 *
 *  02:41:27  IST      0.00   1732.35    158.82    100.00    173.53      0.00      0.00      0.00      0.00
 *  Average:         0.00    240.06    879.35     61.35    683.68      0.00      0.00      0.00      0.00
 *
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

