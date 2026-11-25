/*
 * Practical: Dining Philosophers Synchronization Problem
 * Description:
 *     Five philosophers sit around a table with five forks. Each philosopher alternates
 *     between thinking and eating. A philosopher must acquire both adjacent forks
 *     (left and right) to eat.
 *     Deadlock is prevented using a room semaphore that permits at most N-1 philosophers
 *     to attempt picking up forks simultaneously.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define CYCLES 2

sem_t forks[NUM_PHILOSOPHERS];
sem_t room;

void *philosopher(void *arg) {
    int phil_id = *(int *)arg;
    int left_fork = phil_id;
    int right_fork = (phil_id + 1) % NUM_PHILOSOPHERS;

    for (int cycle = 1; cycle <= CYCLES; cycle++) {
        /* 1. Thinking */
        printf("Philosopher %d is THINKING (Cycle %d)\n", phil_id, cycle);
        usleep((50 + rand() % 100) * 1000);

        /* 2. Enter dining room and acquire forks */
        sem_wait(&room);
        sem_wait(&forks[left_fork]);
        sem_wait(&forks[right_fork]);

        /* 3. Eating */
        printf("  --> Philosopher %d is EATING using forks (%d, %d)\n",
               phil_id, left_fork, right_fork);
        usleep((100 + rand() % 100) * 1000);

        /* 4. Release forks and exit room */
        sem_post(&forks[right_fork]);
        sem_post(&forks[left_fork]);
        sem_post(&room);

        printf("Philosopher %d finished eating and put down forks (%d, %d)\n",
               phil_id, left_fork, right_fork);
    }
    return NULL;
}

int main(void) {
    srand(42);

    printf("=================================================================\n");
    printf("      DINING PHILOSOPHERS SYNCHRONIZATION SIMULATION\n");
    printf("=================================================================\n");
    printf("Philosophers : %d\n", NUM_PHILOSOPHERS);
    printf("Cycles/person: %d\n", CYCLES);
    printf("Deadlock Prevention: Room capacity semaphore (N - 1 = %d)\n", NUM_PHILOSOPHERS - 1);
    printf("-----------------------------------------------------------------\n");

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }
    sem_init(&room, 0, NUM_PHILOSOPHERS - 1);

    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }
    sem_destroy(&room);

    printf("=================================================================\n");
    printf("Simulation Complete: All philosophers completed thinking and eating.\n");
    printf("=================================================================\n");

    return 0;
}

