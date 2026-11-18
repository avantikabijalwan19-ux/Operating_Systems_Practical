# Operating Systems Lab Practicals (Python Implementation)

A comprehensive, modular, and educational repository of all core **Operating Systems Practicals** implemented in clean, idiomatic Python 3. This repository covers CPU scheduling algorithms, classic synchronization problems (inter-process communication), and process creation.

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
5. [How to Run](#how-to-run)
6. [Requirements & Platform Compatibility](#requirements--platform-compatibility)

---

## 🔍 Overview

This lab suite is structured into two major domains:
1. **Uniprocessor CPU Scheduling:**
   - Evaluates performance criteria including Turnaround Time (TAT), Waiting Time (WT), and CPU utilization across preemptive and non-preemptive strategies.
   - Includes real-time ASCII Gantt chart generation.
2. **Process Synchronization & IPC:**
   - Demonstrates mutual exclusion, deadlock avoidance, and thread synchronization using Python's `threading.Semaphore` and `threading.Lock`.
   - Explores process creation, address space isolation, and process hierarchy.

---

## 🗂 Practicals Index

| File Name | Topic | Description |
|---|---|---|
| `fcfs.py` | CPU Scheduling | Non-preemptive scheduling based strictly on arrival time order. |
| `sjf_non_preemptive.py` | CPU Scheduling | Selects the process with the shortest burst time among arrived processes. |
| `srtf_preemptive.py` | CPU Scheduling | Preempts running processes if a newly arrived process has less remaining time. |
| `priority_non_preemptive.py` | CPU Scheduling | Prioritizes processes based on assigned priority integers (lower = higher priority). |
| `priority_preemptive.py` | CPU Scheduling | Preempts the CPU immediately if a process with strictly higher priority arrives. |
| `round_robin.py` | CPU Scheduling | Dispatches processes in a cyclic ready queue with a fixed Time Quantum (TQ). |
| `cpu_scheduling_menu.py` | Dashboard | Menu-driven console app that compares all 6 algorithms side-by-side. |
| `producer_consumer.py` | Synchronization | Bounded buffer coordination using `empty`, `full`, and `mutex` semaphores. |
| `dining_philosophers.py` | Synchronization | Resolves the 5-philosopher dining dilemma and avoids deadlocks via room semaphore. |
| `readers_writers.py` | Synchronization | Reader-preference synchronization allowing multiple concurrent readers. |
| `fork_simulation.py` | Process Management | Demonstrates parent/child process creation, PID tracking, and memory isolation. |

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
- **Mechanism:** Maintained via a FIFO ready queue (`collections.deque`). If a process doesn't finish within the quantum, it is appended to the back of the queue.

### Practical 7: Unified Menu & Comparative Benchmark
- **File:** `cpu_scheduling_menu.py`
- **Feature:** Runs any individual algorithm or tests all 6 algorithms against the exact same dataset to directly contrast their Average TAT and Average WT.

---

### Practical 8: Producer-Consumer Problem
- **Concept:** One producer thread places items into a finite buffer of capacity $N$. One consumer thread retrieves items.
- **Semaphores Used:**
  - `empty_slots = Semaphore(N)`: Tracks free slots.
  - `full_slots = Semaphore(0)`: Tracks occupied slots.
  - `mutex = Lock()`: Ensures mutual exclusion while altering buffer pointers.

### Practical 9: Dining Philosophers Problem
- **Concept:** 5 philosophers sit at a round table with 5 shared forks. Eating requires holding both left and right forks.
- **Deadlock Prevention:** A `room` semaphore initialized to $N-1 = 4$ ensures at least one philosopher is always guaranteed to acquire both forks and eat, breaking circular wait.

### Practical 10: Readers-Writers Problem
- **Concept:** Readers only read (concurrent access allowed). Writers modify data (exclusive access required).
- **Solution:** A `read_count` variable tracks active readers. The first reader acquires the writer lock `wrt`, and the last reader releases `wrt`.

### Practical 11: Process Creation & Fork Simulation
- **Concept:** Shows process creation, independent PID assignment (`os.getpid()`, `os.getppid()`), and memory isolation.
- **Cross-Platform Support:** Works smoothly on Windows (via `multiprocessing`) and UNIX (via native `os.fork()`).

---

## 🚀 How to Run

Run any file directly using Python 3:

```bash
# CPU Scheduling Practicals
python fcfs.py
python sjf_non_preemptive.py
python srtf_preemptive.py
python priority_non_preemptive.py
python priority_preemptive.py
python round_robin.py

# Comparative Menu Dashboard
python cpu_scheduling_menu.py

# Process Synchronization Practicals
python producer_consumer.py
python dining_philosophers.py
python readers_writers.py
python fork_simulation.py
```

*Every script includes pre-configured sample inputs so you can simply press Enter to execute!*

---

## ⚙️ Requirements & Platform Compatibility
- **Python Version:** Python 3.8+ (tested on Python 3.10)
- **Operating System:** Windows, Linux, or macOS.
- **Dependencies:** **Zero external packages required** (Uses Python standard library: `threading`, `multiprocessing`, `collections`, `random`, `time`, `os`, `sys`).

