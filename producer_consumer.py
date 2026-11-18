"""
Practical: Producer-Consumer Problem using Semaphores and Mutex
Description:
    Demonstrates process synchronization on a finite bounded buffer.
    Uses counting semaphores ('empty' and 'full') and a mutual exclusion lock ('mutex')
    to coordinate producer and consumer threads without race conditions or buffer overflows.
"""

import threading
import time
import random

BUFFER_SIZE = 5
TOTAL_ITEMS = 10

buffer = [None] * BUFFER_SIZE
in_idx = 0
out_idx = 0

# Semaphores
empty_slots = threading.Semaphore(BUFFER_SIZE)
full_slots = threading.Semaphore(0)
mutex = threading.Lock()

def producer():
    global in_idx
    for item in range(1, TOTAL_ITEMS + 1):
        time.sleep(random.uniform(0.05, 0.2))  # Simulate item creation time

        empty_slots.acquire()
        with mutex:
            buffer[in_idx] = item
            print(f"[PRODUCER] Produced item {item:>2} at slot {in_idx} | Buffer: {[x for x in buffer if x is not None]}")
            in_idx = (in_idx + 1) % BUFFER_SIZE

        full_slots.release()

def consumer():
    global out_idx
    for _ in range(TOTAL_ITEMS):
        time.sleep(random.uniform(0.08, 0.25))  # Simulate consumption time

        full_slots.acquire()
        with mutex:
            item = buffer[out_idx]
            buffer[out_idx] = None
            print(f"    [CONSUMER] Consumed item {item:>2} from slot {out_idx} | Buffer: {[x for x in buffer if x is not None]}")
            out_idx = (out_idx + 1) % BUFFER_SIZE

        empty_slots.release()

def main():
    print("=" * 65)
    print("     PRODUCER-CONSUMER PROBLEM (SEMAPHORE SYNCHRONIZATION)")
    print("=" * 65)
    print(f"Buffer Capacity : {BUFFER_SIZE}")
    print(f"Items to Produce: {TOTAL_ITEMS}")
    print("-" * 65)

    prod_thread = threading.Thread(target=producer, name="ProducerThread")
    cons_thread = threading.Thread(target=consumer, name="ConsumerThread")

    prod_thread.start()
    cons_thread.start()

    prod_thread.join()
    cons_thread.join()

    print("=" * 65)
    print("Synchronization Complete: All items safely produced and consumed.")
    print("=" * 65)

if __name__ == "__main__":
    main()

