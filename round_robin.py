"""
Practical: Round Robin (RR) CPU Scheduling Algorithm
Description:
    Processes are dispatched in a FIFO ready queue and allocated CPU time in fixed slices
    called Time Quantum (TQ). If a process does not complete within TQ, it is preempted
    and re-enqueued at the back of the ready queue.
"""

from collections import deque

def solve_round_robin(processes, time_quantum):
    n = len(processes)
    procs = [dict(p) for p in processes]
    remaining_bt = [p['bt'] for p in procs]

    # Sort primarily to manage arrival order
    sorted_indices = sorted(range(n), key=lambda i: (procs[i]['at'], procs[i]['pid']))

    current_time = 0
    completed = 0
    ready_queue = deque()
    in_queue = [False] * n
    gantt_chart = []

    # Push first arrived process(es)
    idx = 0
    while idx < n and procs[sorted_indices[idx]]['at'] <= current_time:
        p_i = sorted_indices[idx]
        ready_queue.append(p_i)
        in_queue[p_i] = True
        idx += 1

    while completed < n:
        if not ready_queue:
            # CPU is idle: jump to next arrival
            if idx < n:
                next_p = sorted_indices[idx]
                next_at = procs[next_p]['at']
                gantt_chart.append(('IDLE', current_time, next_at))
                current_time = next_at
                while idx < n and procs[sorted_indices[idx]]['at'] <= current_time:
                    p_i = sorted_indices[idx]
                    ready_queue.append(p_i)
                    in_queue[p_i] = True
                    idx += 1
            continue

        curr = ready_queue.popleft()
        start = current_time
        exec_time = min(remaining_bt[curr], time_quantum)
        current_time += exec_time
        remaining_bt[curr] -= exec_time
        gantt_chart.append((procs[curr]['pid'], start, current_time))

        # Check for newly arrived processes during this time slice
        while idx < n and procs[sorted_indices[idx]]['at'] <= current_time:
            p_i = sorted_indices[idx]
            if not in_queue[p_i]:
                ready_queue.append(p_i)
                in_queue[p_i] = True
            idx += 1

        # If current process has remaining burst, re-enqueue it
        if remaining_bt[curr] > 0:
            ready_queue.append(curr)
        else:
            completed += 1
            procs[curr]['ct'] = current_time
            procs[curr]['tat'] = procs[curr]['ct'] - procs[curr]['at']
            procs[curr]['wt'] = procs[curr]['tat'] - procs[curr]['bt']

    return procs, gantt_chart

def display_results(processes, gantt_chart, time_quantum):
    print("\n" + "=" * 65)
    print(f"ROUND ROBIN SCHEDULING (Time Quantum = {time_quantum})")
    print("=" * 65)
    print(f"{'PID':<8} {'Arrival':<10} {'Burst':<10} {'Complete':<12} {'TAT':<10} {'WT':<10}")
    print("-" * 65)

    total_tat = sum(p['tat'] for p in processes)
    total_wt = sum(p['wt'] for p in processes)

    for p in sorted(processes, key=lambda x: x['pid']):
        print(f"P{p['pid']:<7} {p['at']:<10} {p['bt']:<10} {p['ct']:<12} {p['tat']:<10} {p['wt']:<10}")

    n = len(processes)
    print("-" * 65)
    print(f"Average Turnaround Time (TAT): {total_tat / n:.2f}")
    print(f"Average Waiting Time (WT):    {total_wt / n:.2f}")
    print("=" * 65)

    print("\nGantt Chart:")
    bar = "|"
    timeline = str(gantt_chart[0][1])
    for pid, start, end in gantt_chart:
        label = f" P{pid} " if pid != 'IDLE' else " IDLE "
        bar += f"{label}|"
        timeline += f"{end:>{len(label) + 1}}"
    print("-" * len(bar))
    print(bar)
    print("-" * len(bar))
    print(timeline)
    print()

def main():
    print("=" * 60)
    print("       ROUND ROBIN (RR) CPU SCHEDULING")
    print("=" * 60)
    use_default = input("Use default sample processes? (Y/n): ").strip().lower()

    if use_default == 'n':
        n = int(input("Enter number of processes: "))
        processes = []
        for i in range(n):
            print(f"\nProcess P{i + 1}:")
            at = int(input("  Arrival Time: "))
            bt = int(input("  Burst Time:   "))
            processes.append({'pid': i + 1, 'at': at, 'bt': bt})
        tq = int(input("\nEnter Time Quantum: "))
    else:
        processes = [
            {'pid': 1, 'at': 0, 'bt': 5},
            {'pid': 2, 'at': 1, 'bt': 4},
            {'pid': 3, 'at': 2, 'bt': 2},
            {'pid': 4, 'at': 4, 'bt': 1}
        ]
        tq = 2

    solved, gantt = solve_round_robin(processes, tq)
    display_results(solved, gantt, tq)

if __name__ == "__main__":
    main()

