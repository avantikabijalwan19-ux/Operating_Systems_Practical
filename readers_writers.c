/*
 * Practical: Readers-Writers Synchronization Problem
 * Description:
 *     Demonstrates concurrency control where multiple readers can read a shared resource
 *     simultaneously, but writers require exclusive access (no concurrent readers or writers).
 *     Uses reader-count, mutex lock, and write semaphore.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int shared_data = 100;
int read_count = 0;

pthread_mutex_t mutex; /* Protects read_count */
sem_t wrt;             /* Exclusive access for writers */

void *reader(void *arg) {
    int reader_id = *(int *)arg;
    for (int step = 0; step < 2; step++) {
        usleep((50 + rand() % 150) * 1000);

        /* Reader entry section */
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&wrt); /* First reader locks out writers */
        }
        pthread_mutex_unlock(&mutex);

        /* Reading section */
        printf("[READER %d] Reading shared_data = %d (Active Readers: %d)\n",
               reader_id, shared_data, read_count);
        usleep((50 + rand() % 100) * 1000);

        /* Reader exit section */
        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&wrt); /* Last reader allows writers back in */
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *writer(void *arg) {
    int writer_id = *(int *)arg;
    for (int step = 0; step < 2; step++) {
        usleep((100 + rand() % 150) * 1000);

        /* Writer acquires exclusive write semaphore */
        sem_wait(&wrt);
        shared_data += 10;
        printf("    >>> [WRITER %d] Modified shared_data to %d <<<\n",
               writer_id, shared_data);
        usleep((100 + rand() % 100) * 1000);
        sem_post(&wrt);
    }
    return NULL;
}

int main(void) {
    srand(42);

    printf("=================================================================\n");
    printf("       READERS-WRITERS SYNCHRONIZATION SIMULATION\n");
    printf("=================================================================\n");
    printf("Policy: Reader-Preference (multiple readers allowed concurrently)\n");
    printf("-----------------------------------------------------------------\n");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt, 0, 1);

    pthread_t readers[5], writers[2];
    int r_ids[5], w_ids[2];

    for (int i = 0; i < 5; i++) {
        r_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &r_ids[i]);
    }
    for (int j = 0; j < 2; j++) {
        w_ids[j] = j + 1;
        pthread_create(&writers[j], NULL, writer, &w_ids[j]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int j = 0; j < 2; j++) {
        pthread_join(writers[j], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    printf("=================================================================\n");
    printf("All readers and writers finished. Final shared_data = %d\n", shared_data);
    printf("=================================================================\n");

    return 0;
}

