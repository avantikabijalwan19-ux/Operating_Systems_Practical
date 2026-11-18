"""
Practical: Priority Scheduling Algorithm (Non-Preemptive)
Description:
    Each process has an assigned priority number (Standard: Lower number = Higher priority).
    Among arrived processes, the one with highest priority is allocated the CPU
    and runs to completion non-preemptively.
"""

def solve_priority_non_preemptive(processes):
    n = len(processes)
    completed = 0
    current_time = 0
    is_completed = [False] * n
    gantt_chart = []

    procs = [dict(p) for p in processes]

    while completed < n:
        candidates = [
            (i, procs[i]) for i in range(n)
            if not is_completed[i] and procs[i]['at'] <= current_time
        ]

        if not candidates:
            next_at = min(procs[i]['at'] for i in range(n) if not is_completed[i])
            gantt_chart.append(('IDLE', current_time, next_at))
            current_time = next_at
            continue

        # Lowest priority number = Highest priority
        candidates.sort(key=lambda x: (x[1]['priority'], x[1]['at'], x[1]['pid']))
        chosen_idx, chosen_p = candidates[0]

        start = current_time
        current_time += chosen_p['bt']
        chosen_p['ct'] = current_time
        chosen_p['tat'] = chosen_p['ct'] - chosen_p['at']
        chosen_p['wt'] = chosen_p['tat'] - chosen_p['bt']
        is_completed[chosen_idx] = True
        completed += 1
        gantt_chart.append((chosen_p['pid'], start, current_time))

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
    print("      PRIORITY SCHEDULING (NON-PREEMPTIVE)")
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
            {'pid': 1, 'priority': 2, 'at': 0, 'bt': 4},
            {'pid': 2, 'priority': 1, 'at': 1, 'bt': 3},
            {'pid': 3, 'priority': 4, 'at': 2, 'bt': 1},
            {'pid': 4, 'priority': 3, 'at': 3, 'bt': 5},
            {'pid': 5, 'priority': 5, 'at': 4, 'bt': 2}
        ]

    solved, gantt = solve_priority_non_preemptive(processes)
    display_results(solved, gantt)

if __name__ == "__main__":
    main()

