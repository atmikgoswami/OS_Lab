/************************************************************************************************************************************************************
 * 
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Sep 28 13:12:41 IST 2024
 *      
 *  Assignment No: 7
 *  Assignment Details:
 *  Create a main process which creates three threads Th1, Th2, and Th3. The main process also creates two random quantities (X, Y), both less than 10. 
 *  These two values will be placed by the main process in the shared memory that is accessible by all the three threads Th1, Th2 and Th3. 
 *  The shared memory will be created by the main process also. For each pair of values (X,Y), thread Th1 will compute A (X*Y) and the thread Th2 will 
 *  compute B (X*Y)/2). Similarly,  Th3 computes C (X+Y), Th2 again computes D ((X*Y)/(X+Y)), and finally Th1 computes E ((X+Y)(X-Y)). 
 *  All these values are kept in the shared memory in a tabular fashion.
 *
 *  Input Description: Enter the number of pairs
 *  Output Description: Displays the values of A,B,C,D,E computed by the three threads in tabular fashion.
 *  Compilation command: gcc 1104_7A.c -o Output7A -std=gnu99 -lpthread
 *  Execution sequence: ./Output7A <num_pairs>  		                           
 *                               
 *  Sample Input: [be22104@localhost Assignment7]$ ./Output7A 4
 *  Sample Output:
 *  X       Y       A       B       C       D       E
 *  5       7       35      17      12      2       -24
 *  6       3       18      9       9       2       27
 *  7       4       28      14      11      2       33
 *  3       4       12      6       7       1       -7
 ***************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/* Structure to store a pair (X, Y) and computations (A, B, C, D, E)*/
typedef struct {
    int X, Y;
    int A, B, C, D, E;
} Computation;

Computation *shared_memory; /*Shared memory*/
pthread_barrier_t barrier; /* Thread synchronization barrier variable */
int num_pairs;

void* compute_A_E(void *arg) {
    for (int i = 0; i < num_pairs; i++) {
        pthread_barrier_wait(&barrier); /* Synchronize before starting calculation for the pair */

        shared_memory[i].A = shared_memory[i].X * shared_memory[i].Y;
        shared_memory[i].E = (shared_memory[i].X + shared_memory[i].Y) * (shared_memory[i].X - shared_memory[i].Y);

        pthread_barrier_wait(&barrier); /* Synchronize before moving to next pair */
    }
    return NULL;
}

void* compute_B_D(void *arg) {
    for (int i = 0; i < num_pairs; i++) {
        pthread_barrier_wait(&barrier);

        shared_memory[i].B = (shared_memory[i].X * shared_memory[i].Y) / 2;
        if (shared_memory[i].X + shared_memory[i].Y != 0) {  /* Avoid division by zero */
            shared_memory[i].D = (shared_memory[i].X * shared_memory[i].Y) / (shared_memory[i].X + shared_memory[i].Y);
        } else {
            shared_memory[i].D = 0;
        }

        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

void* compute_C(void *arg) {
    for (int i = 0; i < num_pairs; i++) {
        pthread_barrier_wait(&barrier);

        shared_memory[i].C = shared_memory[i].X + shared_memory[i].Y;

        pthread_barrier_wait(&barrier);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_pairs>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    num_pairs = atoi(argv[1]);
    if (num_pairs <= 0) {
        fprintf(stderr, "Please enter a positive number of pairs.\n");
        exit(EXIT_FAILURE);
    }

    /* Create shared memory for storing (X, Y) pairs and results */
    int shm_fd = shm_open("/my_shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(Computation) * num_pairs);
    shared_memory = mmap(0, sizeof(Computation) * num_pairs, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    /* Initialize random number generator */
    srand(time(NULL));
    for (int i = 0; i < num_pairs; i++) {
        shared_memory[i].X = rand() % 10;
        shared_memory[i].Y = rand() % 10;
    }

    /* Initialize barrier for synchronization */
    pthread_barrier_init(&barrier, NULL, 3);

    /* Create threads */
    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, compute_A_E, NULL);
    pthread_create(&th2, NULL, compute_B_D, NULL);
    pthread_create(&th3, NULL, compute_C, NULL);

    /* Wait for threads to complete */
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    printf("X\tY\tA\tB\tC\tD\tE\n");
    for (int i = 0; i < num_pairs; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", shared_memory[i].X, shared_memory[i].Y,
               shared_memory[i].A, shared_memory[i].B, shared_memory[i].C,
               shared_memory[i].D, shared_memory[i].E);
    }

    /* Clean up */
    pthread_barrier_destroy(&barrier);
    shm_unlink("/my_shared_memory");
    return 0;
}
