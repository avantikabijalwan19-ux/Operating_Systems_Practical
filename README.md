# Operating Systems Lab Practicals (C Implementation)

A comprehensive, modular, and educational repository of all core **Operating Systems Practicals** implemented in clean, standard C. This repository covers CPU scheduling algorithms, classic synchronization problems (inter-process communication), and process creation.

---

## 📚 Table of Contents
1. [Overview](#overview)
2. [Practicals Index](#practicals-index)
3. [CPU Scheduling Formulas & Metrics](#cpu-scheduling-formulas--metrics)
4. [Detailed Practical Descriptions](#detailed-practical-descriptions)
   - [Practical 1: First-Come, First-Served (FCFS)](#practical-1-first-come-first-served-fcfs)
   - [Practical 2: Shortest Job First (SJF Non-Preemptive)](#practical-2-shortest-job-first-sjf-non-preemptive)
   - [Practical 3: Shortest Remaining Time First (SRTF Preemptive)](#practical-3-shortest-remaining-time-first-srtf-preemptive)
   - [Practical 4: Priority Scheduling (Non-Preemptive)](#practical-4-priority-scheduling-non-preemptive)
   - [Practical 5: Priority Scheduling (Preemptive)](#practical-5-priority-scheduling-preemptive)
   - [Practical 6: Round Robin Scheduling (RR)](#practical-6-round-robin-scheduling-rr)
   - [Practical 7: Unified Menu & Comparative Benchmark](#practical-7-unified-menu--comparative-benchmark)
   - [Practical 8: Producer-Consumer Problem](#practical-8-producer-consumer-problem)
   - [Practical 9: Dining Philosophers Problem](#practical-9-dining-philosophers-problem)
   - [Practical 10: Readers-Writers Problem](#practical-10-readers-writers-problem)
   - [Practical 11: Process Creation & Fork Simulation](#practical-11-process-creation--fork-simulation)
5. [How to Compile & Run](#how-to-compile--run)
6. [Requirements & Platform Compatibility](#requirements--platform-compatibility)

---

## 🔍 Overview

This lab suite is structured into two major domains:
1. **Uniprocessor CPU Scheduling:**
   - Evaluates performance criteria including Turnaround Time (TAT), Waiting Time (WT), and CPU utilization across preemptive and non-preemptive strategies.
   - Includes real-time ASCII Gantt chart generation.
2. **Process Synchronization & IPC:**
   - Demonstrates mutual exclusion, deadlock avoidance, and thread synchronization using POSIX semaphores (`sem_t`) and mutexes (`pthread_mutex_t`).
   - Explores process creation, address space isolation, and process hierarchy (`fork()` on POSIX / `CreateProcess()` on Windows).

---

## 🗂 Practicals Index

| File Name | Topic | Description |
|---|---|---|
| `fcfs.c` | CPU Scheduling | Non-preemptive scheduling based strictly on arrival time order. |
| `sjf_non_preemptive.c` | CPU Scheduling | Selects the process with the shortest burst time among arrived processes. |
| `srtf_preemptive.c` | CPU Scheduling | Preempts running processes if a newly arrived process has less remaining time. |
| `priority_non_preemptive.c` | CPU Scheduling | Prioritizes processes based on assigned priority integers (lower = higher priority). |
| `priority_preemptive.c` | CPU Scheduling | Preempts the CPU immediately if a process with strictly higher priority arrives. |
| `round_robin.c` | CPU Scheduling | Dispatches processes in a cyclic ready queue with a fixed Time Quantum (TQ). |
| `cpu_scheduling_menu.c` | Dashboard | Menu-driven console app that compares all 6 algorithms side-by-side. |
| `producer_consumer.c` | Synchronization | Bounded buffer coordination using `empty_slots`, `full_slots`, and `mutex`. |
| `dining_philosophers.c` | Synchronization | Resolves the 5-philosopher dining dilemma and avoids deadlocks via room semaphore. |
| `readers_writers.c` | Synchronization | Reader-preference synchronization allowing multiple concurrent readers. |
| `fork_simulation.c` | Process Management | Demonstrates parent/child process creation, PID tracking, and memory isolation. |

---

## 📐 CPU Scheduling Formulas & Metrics

For any process $P_i$:
- **Arrival Time ($AT$):** Time at which the process enters the Ready Queue.
- **Burst Time ($BT$):** Total CPU execution time required by the process.
- **Completion Time ($CT$):** Time at which the process finishes execution.
- **Turnaround Time ($TAT$):** Total elapsed time from arrival to completion.
  $$\text{Turnaround Time (TAT)} = CT - AT$$
- **Waiting Time ($WT$):** Total time spent waiting in the ready queue.
  $$\text{Waiting Time (WT)} = TAT - BT$$
- **Average Metrics:**
  $$\text{Avg TAT} = \frac{1}{n} \sum_{i=1}^n TAT_i \quad , \quad \text{Avg WT} = \frac{1}{n} \sum_{i=1}^n WT_i$$

---

## 📖 Detailed Practical Descriptions

### Practical 1: First-Come, First-Served (FCFS)
- **Concept:** Non-preemptive. The CPU is allocated to processes in the exact order of their arrival.
- **Characteristic:** Simple, but subject to the *Convoy Effect* where short processes wait behind very long CPU-bound jobs.

### Practical 2: Shortest Job First (SJF Non-Preemptive)
- **Concept:** When the CPU is free, it selects the arrived process with the smallest execution burst.
- **Characteristic:** Non-preemptive; once scheduled, it executes to completion.

### Practical 3: Shortest Remaining Time First (SRTF Preemptive)
- **Concept:** Preemptive version of SJF. At each clock tick, the scheduler checks if any newly arrived job has a remaining burst time smaller than the currently executing job.
- **Characteristic:** Yields the theoretical minimum average waiting time among all scheduling algorithms.

### Practical 4 & 5: Priority Scheduling (Non-Preemptive & Preemptive)
- **Concept:** Each process is assigned an integer priority (Standard academic convention: **Lower numerical value = Higher priority**).
- **Non-Preemptive:** Highest priority job executes to completion.
- **Preemptive:** If a process with higher priority arrives, it preempts the currently running process.

### Practical 6: Round Robin Scheduling (RR)
- **Concept:** Fair-share preemptive scheduling. Each process receives a slice of time called the **Time Quantum (TQ)**.
- **Mechanism:** Maintained via a circular FIFO ready queue. If a process doesn't finish within the quantum, it is appended to the back of the queue.

### Practical 7: Unified Menu & Comparative Benchmark
- **File:** `cpu_scheduling_menu.c`
- **Feature:** Runs any individual algorithm or tests all 6 algorithms against the exact same dataset to directly contrast their Average TAT and Average WT.

---

### Practical 8: Producer-Consumer Problem
- **Concept:** One producer thread places items into a finite buffer of capacity $N$. One consumer thread retrieves items.
- **Semaphores Used:**
  - `empty_slots` (initialized to $N$): Tracks free slots.
  - `full_slots` (initialized to $0$): Tracks occupied slots.
  - `mutex` (`pthread_mutex_t`): Ensures mutual exclusion while altering buffer indices.

### Practical 9: Dining Philosophers Problem
- **Concept:** 5 philosophers sit at a round table with 5 shared forks. Eating requires holding both left and right forks.
- **Deadlock Prevention:** A `room` semaphore initialized to $N-1 = 4$ ensures at least one philosopher is always guaranteed to acquire both forks and eat, breaking circular wait.

### Practical 10: Readers-Writers Problem
- **Concept:** Readers only read (concurrent access allowed). Writers modify data (exclusive access required).
- **Solution:** A `read_count` variable tracks active readers. The first reader acquires the writer semaphore `wrt`, and the last reader posts `wrt`.

### Practical 11: Process Creation & Fork Simulation
- **Concept:** Shows process creation, independent PID assignment (`getpid()`, `getppid()`), and memory isolation.
- **Cross-Platform Support:** Uses native `fork()` on UNIX/Linux/macOS and `CreateProcess()` on Windows.

---

## 🚀 How to Compile & Run

Compile and run any file using `gcc`:

```bash
# CPU Scheduling Practicals
gcc fcfs.c -o fcfs && ./fcfs
gcc sjf_non_preemptive.c -o sjf_non_preemptive && ./sjf_non_preemptive
gcc srtf_preemptive.c -o srtf_preemptive && ./srtf_preemptive
gcc priority_non_preemptive.c -o priority_non_preemptive && ./priority_non_preemptive
gcc priority_preemptive.c -o priority_preemptive && ./priority_preemptive
gcc round_robin.c -o round_robin && ./round_robin

# Comparative Menu Dashboard
gcc cpu_scheduling_menu.c -o cpu_scheduling_menu && ./cpu_scheduling_menu

# Process Synchronization Practicals (link with -lpthread)
gcc producer_consumer.c -o producer_consumer -lpthread && ./producer_consumer
gcc dining_philosophers.c -o dining_philosophers -lpthread && ./dining_philosophers
gcc readers_writers.c -o readers_writers -lpthread && ./readers_writers

# Process Creation & Fork Simulation
gcc fork_simulation.c -o fork_simulation && ./fork_simulation
```

*Every CPU scheduling program includes pre-configured sample inputs so you can simply press Enter to execute!*

---

## ⚙️ Requirements & Platform Compatibility
- **Compiler:** GCC or Clang supporting C99/C11.
- **Operating System:** Linux, macOS, or Windows (MinGW-w64 / MSYS2 / WSL for POSIX threads & semaphores).
- **Dependencies:** Standard C library (`stdio.h`, `stdlib.h`, `string.h`) and POSIX Threads (`pthread.h`, `semaphore.h`) for synchronization practicals.
