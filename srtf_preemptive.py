"""
Practical: Shortest Remaining Time First (SRTF) CPU Scheduling Algorithm (Preemptive SJF)
Description:
    Preemptive variant of SJF. At every unit of time, the CPU is allocated to the
    process with the smallest remaining execution time. Preempts the running process
    if a newly arrived process requires less remaining time.
"""

def solve_srtf(processes):
    n = len(processes)
    procs = [dict(p) for p in processes]
    remaining_bt = [p['bt'] for p in procs]

    current_time = 0
    completed = 0
    raw_timeline = []  # stores (pid, unit_time)

    while completed < n:
        # Arrived processes with remaining time > 0
        candidates = [
            i for i in range(n)
            if procs[i]['at'] <= current_time and remaining_bt[i] > 0
        ]

        if not candidates:
            raw_timeline.append(('IDLE', current_time))
            current_time += 1
            continue

        # Choose the process with minimum remaining burst time
        candidates.sort(key=lambda idx: (remaining_bt[idx], procs[idx]['at'], procs[idx]['pid']))
        chosen = candidates[0]

        raw_timeline.append((procs[chosen]['pid'], current_time))
        remaining_bt[chosen] -= 1
        current_time += 1

        if remaining_bt[chosen] == 0:
            completed += 1
            procs[chosen]['ct'] = current_time
            procs[chosen]['tat'] = procs[chosen]['ct'] - procs[chosen]['at']
            procs[chosen]['wt'] = procs[chosen]['tat'] - procs[chosen]['bt']

    # Compress raw unit-time timeline into Gantt Chart segments (pid, start, end)
    gantt_chart = []
    if raw_timeline:
        curr_pid, start_time = raw_timeline[0]
        curr_len = 1
        for pid, t in raw_timeline[1:]:
            if pid == curr_pid:
                curr_len += 1
            else:
                gantt_chart.append((curr_pid, start_time, start_time + curr_len))
                curr_pid = pid
                start_time = t
                curr_len = 1
        gantt_chart.append((curr_pid, start_time, start_time + curr_len))

    return procs, gantt_chart

def display_results(processes, gantt_chart):
    print("\n" + "=" * 65)
    print(f"{'PID':<8} {'Arrival':<10} {'Burst':<10} {'Complete':<12} {'TAT':<10} {'WT':<10}")
    print("=" * 65)

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
    print("  SHORTEST REMAINING TIME FIRST (SRTF) PREEMPTIVE SCHEDULING")
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
    else:
        processes = [
            {'pid': 1, 'at': 0, 'bt': 8},
            {'pid': 2, 'at': 1, 'bt': 4},
            {'pid': 3, 'at': 2, 'bt': 2},
            {'pid': 4, 'at': 3, 'bt': 1},
            {'pid': 5, 'at': 4, 'bt': 3}
        ]

    solved, gantt = solve_srtf(processes)
    display_results(solved, gantt)

if __name__ == "__main__":
    main()

