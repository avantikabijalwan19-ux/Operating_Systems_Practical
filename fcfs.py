"""
Practical: First-Come, First-Served (FCFS) CPU Scheduling Algorithm
Description:
    Processes are executed strictly in the order of their Arrival Time (AT).
    Computes Completion Time (CT), Turnaround Time (TAT = CT - AT),
    and Waiting Time (WT = TAT - BT), along with an ASCII Gantt Chart.
"""

def solve_fcfs(processes):
    # processes: list of dicts [{'pid': 1, 'at': 0, 'bt': 4}, ...]
    # Sort processes by Arrival Time
    sorted_procs = sorted(processes, key=lambda p: (p['at'], p['pid']))

    current_time = 0
    gantt_chart = []  # tuples of (pid, start_time, end_time)

    for p in sorted_procs:
        if current_time < p['at']:
            # CPU is idle
            gantt_chart.append(('IDLE', current_time, p['at']))
            current_time = p['at']

        start = current_time
        current_time += p['bt']
        p['ct'] = current_time
        p['tat'] = p['ct'] - p['at']
        p['wt'] = p['tat'] - p['bt']
        gantt_chart.append((p['pid'], start, current_time))

    return sorted_procs, gantt_chart

def display_results(processes, gantt_chart):
    print("\n" + "=" * 65)
    print(f"{'PID':<8} {'Arrival':<10} {'Burst':<10} {'Complete':<12} {'TAT':<10} {'WT':<10}")
    print("=" * 65)

    total_tat = 0
    total_wt = 0

    for p in sorted(processes, key=lambda x: x['pid']):
        total_tat += p['tat']
        total_wt += p['wt']
        print(f"P{p['pid']:<7} {p['at']:<10} {p['bt']:<10} {p['ct']:<12} {p['tat']:<10} {p['wt']:<10}")

    n = len(processes)
    avg_tat = total_tat / n
    avg_wt = total_wt / n

    print("-" * 65)
    print(f"Average Turnaround Time (TAT): {avg_tat:.2f}")
    print(f"Average Waiting Time (WT):    {avg_wt:.2f}")
    print("=" * 65)

    # Gantt Chart
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
    print("    FIRST-COME, FIRST-SERVED (FCFS) CPU SCHEDULING")
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
            {'pid': 1, 'at': 0, 'bt': 4},
            {'pid': 2, 'at': 1, 'bt': 3},
            {'pid': 3, 'at': 2, 'bt': 1},
            {'pid': 4, 'at': 3, 'bt': 2},
            {'pid': 5, 'at': 4, 'bt': 5}
        ]

    solved, gantt = solve_fcfs(processes)
    display_results(solved, gantt)

if __name__ == "__main__":
    main()

