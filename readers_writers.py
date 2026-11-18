"""
Practical: Readers-Writers Synchronization Problem
Description:
    Demonstrates concurrency control where multiple readers can read a shared resource
    simultaneously, but writers require exclusive access (no concurrent readers or writers).
    Uses reader-count, mutex lock, and write lock.
"""

import threading
import time
import random

shared_data = 100
read_count = 0

mutex = threading.Lock()   # Protects read_count
wrt = threading.Lock()     # Exclusive access for writers

def reader(reader_id):
    global read_count
    for _ in range(2):
        time.sleep(random.uniform(0.05, 0.2))

        # Reader entry section
        with mutex:
            read_count += 1
            if read_count == 1:
                wrt.acquire()  # First reader locks out writers

        # Reading section
        print(f"[READER {reader_id}] Reading shared_data = {shared_data} (Active Readers: {read_count})")
        time.sleep(random.uniform(0.05, 0.15))

        # Reader exit section
        with mutex:
            read_count -= 1
            if read_count == 0:
                wrt.release()  # Last reader allows writers back in

def writer(writer_id):
    global shared_data
    for step in range(2):
        time.sleep(random.uniform(0.1, 0.25))

        # Writer acquires exclusive write lock
        wrt.acquire()
        shared_data += 10
        print(f"    >>> [WRITER {writer_id}] Modified shared_data to {shared_data} <<<")
        time.sleep(random.uniform(0.1, 0.2))
        wrt.release()

def main():
    print("=" * 65)
    print("       READERS-WRITERS SYNCHRONIZATION SIMULATION")
    print("=" * 65)
    print("Policy: Reader-Preference (multiple readers allowed concurrently)")
    print("-" * 65)

    readers = [threading.Thread(target=reader, args=(i + 1,)) for i in range(5)]
    writers = [threading.Thread(target=writer, args=(j + 1,)) for j in range(2)]

    threads = readers + writers
    random.shuffle(threads)

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    print("=" * 65)
    print(f"All readers and writers finished. Final shared_data = {shared_data}")
    print("=" * 65)

if __name__ == "__main__":
    main()

