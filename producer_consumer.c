/*
 * Practical: Producer-Consumer Problem using Semaphores and Mutex
 * Description:
 *     Demonstrates process synchronization on a finite bounded buffer.
 *     Uses POSIX counting semaphores ('empty_slots' and 'full_slots') and a
 *     pthread mutex ('mutex') to coordinate producer and consumer threads
 *     without race conditions or buffer overflows.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 10

int buffer[BUFFER_SIZE];
int in_idx = 0;
int out_idx = 0;

sem_t empty_slots;
sem_t full_slots;
pthread_mutex_t mutex;

static void print_buffer(void) {
    printf("[");
    int first = 1;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] != 0) {
            if (!first) printf(", ");
            printf("%d", buffer[i]);
            first = 0;
        }
    }
    printf("]");
}

void *producer(void *arg) {
    (void)arg;
    for (int item = 1; item <= TOTAL_ITEMS; item++) {
        usleep((50 + rand() % 150) * 1000);

        sem_wait(&empty_slots);
        pthread_mutex_lock(&mutex);

        buffer[in_idx] = item;
        printf("[PRODUCER] Produced item %2d at slot %d | Buffer: ", item, in_idx);
        print_buffer();
        putchar('\n');
        in_idx = (in_idx + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full_slots);
    }
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < TOTAL_ITEMS; i++) {
        usleep((80 + rand() % 170) * 1000);

        sem_wait(&full_slots);
        pthread_mutex_lock(&mutex);

        int item = buffer[out_idx];
        buffer[out_idx] = 0;
        printf("    [CONSUMER] Consumed item %2d from slot %d | Buffer: ", item, out_idx);
        print_buffer();
        putchar('\n');
        out_idx = (out_idx + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);
    }
    return NULL;
}

int main(void) {
    srand(42);

    printf("=================================================================\n");
    printf("     PRODUCER-CONSUMER PROBLEM (SEMAPHORE SYNCHRONIZATION)\n");
    printf("=================================================================\n");
    printf("Buffer Capacity : %d\n", BUFFER_SIZE);
    printf("Items to Produce: %d\n", TOTAL_ITEMS);
    printf("-----------------------------------------------------------------\n");

    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&full_slots, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&mutex);

    printf("=================================================================\n");
    printf("Synchronization Complete: All items safely produced and consumed.\n");
    printf("=================================================================\n");

    return 0;
}

