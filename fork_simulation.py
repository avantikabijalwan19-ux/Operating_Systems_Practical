"""
Practical: Process Creation and Management (Fork Simulation)
Description:
    Demonstrates process creation and process identification (PID and Parent PID).
    In UNIX/Linux C, this is done via the 'fork()' system call.
    In Python, this script demonstrates:
      1. Native os.fork() if running on a POSIX/UNIX system.
      2. The cross-platform 'multiprocessing' module (compatible with Windows, macOS, Linux).
      3. Process address space isolation (memory in child is distinct from parent).
"""

import os
import sys
import multiprocessing
import time

shared_variable = 100

def child_process_worker(val):
    global shared_variable
    print("\n[CHILD PROCESS]")
    print(f"  Child Process PID  : {os.getpid()}")
    print(f"  Parent Process PID : {os.getppid()}")
    shared_variable += 50
    print(f"  Child modified shared_variable to: {shared_variable}")
    time.sleep(0.5)
    print("  Child process exiting.")

def run_multiprocessing_demo():
    global shared_variable
    print("\n--- Running Cross-Platform Process Creation (multiprocessing) ---")
    print(f"[PARENT PROCESS]")
    print(f"  Parent PID: {os.getpid()}")
    print(f"  Initial shared_variable: {shared_variable}")

    p = multiprocessing.Process(target=child_process_worker, args=(shared_variable,))
    p.start()
    p.join()

    print("\n[BACK TO PARENT PROCESS]")
    print(f"  Parent PID: {os.getpid()}")
    print(f"  shared_variable in Parent remains: {shared_variable}")
    print("  --> Proves process address spaces are isolated (independent memory copies)!\n")

def run_posix_fork_demo():
    print("\n--- Running Native os.fork() (POSIX Systems) ---")
    pid = os.fork()
    if pid == 0:
        # Child process
        print("[CHILD]")
        print(f"  My PID     = {os.getpid()}")
        print(f"  Parent PID = {os.getppid()}")
        sys.exit(0)
    else:
        # Parent process
        print("[PARENT]")
        print(f"  My PID     = {os.getpid()}")
        print(f"  Child PID  = {pid}")
        os.waitpid(pid, 0)

def main():
    print("=" * 65)
    print("      PROCESS CREATION AND FORK SIMULATION IN PYTHON")
    print("=" * 65)
    print("System Information:")
    print(f"  Operating System: {sys.platform}")
    print(f"  Has os.fork()   : {hasattr(os, 'fork')}")
    print("-" * 65)

    if hasattr(os, 'fork'):
        run_posix_fork_demo()
    else:
        print("Note: Windows does not implement POSIX fork().")
        print("Using Python's multiprocessing engine for true OS-level process creation.")
        run_multiprocessing_demo()

    print("=" * 65)
    print("Theory Summary:")
    print("  1. fork() creates a new process by duplicating the calling process.")
    print("  2. In child:  fork() returns 0.")
    print("  3. In parent: fork() returns PID of the newly created child.")
    print("  4. Both parent and child run concurrently with private memory spaces.")
    print("=" * 65)

if __name__ == "__main__":
    multiprocessing.freeze_support()
    main()

