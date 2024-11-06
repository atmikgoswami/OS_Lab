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
                pthread_mutex_lock(&mutex1);
                usleep(100);  /* Small delay to increase chance of deadlock */
                pthread_mutex_lock(&mutex2);

                /* Perform operation */
                int quantity = generate_random();
                Total_2 -= quantity;
                Total_1 += quantity;
                printf("Thread 2: Total_1 = %d, Total_2 = %d\n", Total_1, Total_2);
                printf("Sum of Totals: %d\n", Total_1 + Total_2);

                /* Unlock mutexes */
                pthread_mutex_unlock(&mutex2);
                pthread_mutex_unlock(&mutex1);

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
