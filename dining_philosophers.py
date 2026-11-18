"""
Practical: Dining Philosophers Synchronization Problem
Description:
    Five philosophers sit around a table with five forks. Each philosopher alternates
    between thinking and eating. A philosopher must acquire both adjacent forks
    (left and right) to eat.
    Deadlock is prevented using a room semaphore that permits at most N-1 philosophers
    to attempt picking up forks simultaneously.
"""

import threading
import time
import random

NUM_PHILOSOPHERS = 5
CYCLES = 2

forks = [threading.Semaphore(1) for _ in range(NUM_PHILOSOPHERS)]
room = threading.Semaphore(NUM_PHILOSOPHERS - 1)  # Room capacity: at most 4 philosophers

def philosopher(phil_id):
    left_fork = phil_id
    right_fork = (phil_id + 1) % NUM_PHILOSOPHERS

    for cycle in range(1, CYCLES + 1):
        # 1. Thinking
        print(f"Philosopher {phil_id} is THINKING (Cycle {cycle})")
        time.sleep(random.uniform(0.05, 0.15))

        # 2. Enter dining room and acquire forks
        room.acquire()
        forks[left_fork].acquire()
        forks[right_fork].acquire()

        # 3. Eating
        print(f"  --> Philosopher {phil_id} is EATING using forks ({left_fork}, {right_fork})")
        time.sleep(random.uniform(0.1, 0.2))

        # 4. Release forks and exit room
        forks[right_fork].release()
        forks[left_fork].release()
        room.release()

        print(f"Philosopher {phil_id} finished eating and put down forks ({left_fork}, {right_fork})")

def main():
    print("=" * 65)
    print("      DINING PHILOSOPHERS SYNCHRONIZATION SIMULATION")
    print("=" * 65)
    print(f"Philosophers : {NUM_PHILOSOPHERS}")
    print(f"Cycles/person: {CYCLES}")
    print("Deadlock Prevention: Room capacity semaphore (N - 1 = 4)")
    print("-" * 65)

    threads = []
    for i in range(NUM_PHILOSOPHERS):
        t = threading.Thread(target=philosopher, args=(i,), name=f"Phil-{i}")
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    print("=" * 65)
    print("Simulation Complete: All philosophers completed thinking and eating.")
    print("=" * 65)

if __name__ == "__main__":
    main()

