/***********************************************************************************************************************************************
 * 
 *  Name: Atmik Goswami
 *  Roll: 002211001104
 *  Date: Mon Sep 23 13:49:43 IST 2024
 *      
 *  Assignment No: 5
 *  Assignment Details:
 *  The objective of this assignment is to create a deadlock. For this purpose we define two global variables (Total_1 and Total_2) 
 *  and initialize both of them to 1000. We have two mutexes to protect these two global variables. We create two threads also. 
 *  The function of each of the threads is to generate a random quantity (not more than 50) and subtract that quantity from one of the Total 
 *  and add that quantity to the other Total. 
 *  While manipulating the Totals, each Thread should lock both the mutex and then unlock it after changing the Totals. 
 *
 *  Input Description: None
 *  Output Description: No output since the two threads are in deadlock. To prevent deadlock, we can reverse the order of mutex locking.
 *  Compilation command: gcc 1104_5A.c -o Output5A -std=gnu99 -lpthread
 *  Execution sequence: ./Output5A                 
 *                               
 *  Sample Input: None
 *  Sample Output:
 *  [be22104@localhost Assignment5]$ ./Output5A
 *  ^C
 *  [be22104@localhost Assignment5]$
 ***********************************************************************************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int Total_1 = 1000;
int Total_2 = 1000;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

int generate_random() {
        return rand() % 50 + 1;
}

void* thread_func1(void* arg) {
        while (1) {
                /* Lock mutex1 first, then mutex2 */
                pthread_mutex_lock(&mutex1);
                usleep(100);  /* Small delay to increase chance of deadlock */
                pthread_mutex_lock(&mutex2);

                /* Perform operation */
                int quantity = generate_random();
                Total_1 -= quantity;
                Total_2 += quantity;
                printf("Thread 1: Total_1 = %d, Total_2 = %d\n", Total_1, Total_2);
                printf("Sum of Totals: %d\n", Total_1 + Total_2);

                /* Unlock mutexes */
                pthread_mutex_unlock(&mutex2);
                pthread_mutex_unlock(&mutex1);

                usleep(100);  /* Small delay to prevent fast looping */
        }
}

void* thread_func2(void* arg) {
        while (1) {
                /* Lock mutex2 first, then mutex1 (opposite order from thread_func1) */
                pthread_mutex_lock(&mutex2);
                usleep(100);  /* Small delay to increase chance of deadlock */
                pthread_mutex_lock(&mutex1);

                /* Perform operation */
                int quantity = generate_random();
                Total_2 -= quantity;
                Total_1 += quantity;
                printf("Thread 2: Total_1 = %d, Total_2 = %d\n", Total_1, Total_2);
                printf("Sum of Totals: %d\n", Total_1 + Total_2);

                /* Unlock mutexes */
                pthread_mutex_unlock(&mutex1);
                pthread_mutex_unlock(&mutex2);

                usleep(100);  /* Small delay to prevent fast looping */
        }
}

int main() {
        pthread_t thread1, thread2;

        /* Create two threads */
        pthread_create(&thread1, NULL, thread_func1, NULL);
        pthread_create(&thread2, NULL, thread_func2, NULL);

        /* Wait for the threads to complete (they won't due to infinite loop) */
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        return 0;
}
