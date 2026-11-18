"""
Practical: Priority Scheduling Algorithm (Preemptive)
Description:
    Preemptive variant of Priority Scheduling.
    At every clock unit, the CPU executes the arrived process having the highest priority
    (lowest numerical value). If a newly arrived process has higher priority, it preempts
    the currently executing process immediately.
"""

def solve_priority_preemptive(processes):
    n = len(processes)
    procs = [dict(p) for p in processes]
    remaining_bt = [p['bt'] for p in procs]

    current_time = 0
    completed = 0
    raw_timeline = []

    while completed < n:
        candidates = [
            i for i in range(n)
            if procs[i]['at'] <= current_time and remaining_bt[i] > 0
        ]

        if not candidates:
            raw_timeline.append(('IDLE', current_time))
            current_time += 1
            continue

        # Lowest priority number = Highest priority
        candidates.sort(key=lambda idx: (procs[idx]['priority'], procs[idx]['at'], procs[idx]['pid']))
        chosen = candidates[0]

        raw_timeline.append((procs[chosen]['pid'], current_time))
        remaining_bt[chosen] -= 1
        current_time += 1

        if remaining_bt[chosen] == 0:
            completed += 1
            procs[chosen]['ct'] = current_time
            procs[chosen]['tat'] = procs[chosen]['ct'] - procs[chosen]['at']
            procs[chosen]['wt'] = procs[chosen]['tat'] - procs[chosen]['bt']

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
    print("\n" + "=" * 75)
    print(f"{'PID':<6} {'Priority':<10} {'Arrival':<10} {'Burst':<10} {'Complete':<12} {'TAT':<10} {'WT':<10}")
    print("=" * 75)

    total_tat = sum(p['tat'] for p in processes)
    total_wt = sum(p['wt'] for p in processes)

    for p in sorted(processes, key=lambda x: x['pid']):
        print(f"P{p['pid']:<5} {p['priority']:<10} {p['at']:<10} {p['bt']:<10} {p['ct']:<12} {p['tat']:<10} {p['wt']:<10}")

    n = len(processes)
    print("-" * 75)
    print(f"Average Turnaround Time (TAT): {total_tat / n:.2f}")
    print(f"Average Waiting Time (WT):    {total_wt / n:.2f}")
    print("=" * 75)

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
    print("=" * 65)
    print("        PRIORITY SCHEDULING (PREEMPTIVE)")
    print("=" * 65)
    print("Note: Lower priority number indicates higher priority (e.g. 1 > 2).")
    use_default = input("Use default sample processes? (Y/n): ").strip().lower()

    if use_default == 'n':
        n = int(input("Enter number of processes: "))
        processes = []
        for i in range(n):
            print(f"\nProcess P{i + 1}:")
            prio = int(input("  Priority (integer): "))
            at = int(input("  Arrival Time:       "))
            bt = int(input("  Burst Time:         "))
            processes.append({'pid': i + 1, 'priority': prio, 'at': at, 'bt': bt})
    else:
        processes = [
            {'pid': 1, 'priority': 3, 'at': 0, 'bt': 4},
            {'pid': 2, 'priority': 1, 'at': 1, 'bt': 5},
            {'pid': 3, 'priority': 4, 'at': 2, 'bt': 1},
            {'pid': 4, 'priority': 2, 'at': 3, 'bt': 2}
        ]

    solved, gantt = solve_priority_preemptive(processes)
    display_results(solved, gantt)

if __name__ == "__main__":
    main()

