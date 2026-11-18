"""
Practical: Interactive Menu-Driven CPU Scheduling Suite and Comparative Benchmark
Description:
    A unified CLI dashboard integrating FCFS, SJF, SRTF, Priority (Non-Preemptive & Preemptive),
    and Round Robin. Allows running individual algorithms or generating a side-by-side
    performance comparison (Average TAT & Average WT).
"""

from fcfs import solve_fcfs
from sjf_non_preemptive import solve_sjf_non_preemptive
from srtf_preemptive import solve_srtf
from priority_non_preemptive import solve_priority_non_preemptive
from priority_preemptive import solve_priority_preemptive
from round_robin import solve_round_robin

def get_default_processes():
    return [
        {'pid': 1, 'at': 0, 'bt': 7, 'priority': 3},
        {'pid': 2, 'at': 2, 'bt': 4, 'priority': 1},
        {'pid': 3, 'at': 4, 'bt': 1, 'priority': 4},
        {'pid': 4, 'at': 5, 'bt': 4, 'priority': 2}
    ]

def print_gantt_and_table(algo_name, processes, gantt_chart):
    print("\n" + "=" * 70)
    print(f"Algorithm: {algo_name}")
    print("=" * 70)
    print(f"{'PID':<8} {'Arrival':<10} {'Burst':<10} {'Complete':<12} {'TAT':<10} {'WT':<10}")
    print("-" * 70)
    total_tat = sum(p['tat'] for p in processes)
    total_wt = sum(p['wt'] for p in processes)
    for p in sorted(processes, key=lambda x: x['pid']):
        print(f"P{p['pid']:<7} {p['at']:<10} {p['bt']:<10} {p['ct']:<12} {p['tat']:<10} {p['wt']:<10}")

    n = len(processes)
    avg_tat = total_tat / n
    avg_wt = total_wt / n
    print("-" * 70)
    print(f"Average Turnaround Time: {avg_tat:.2f}")
    print(f"Average Waiting Time:    {avg_wt:.2f}")

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
    return avg_tat, avg_wt

def compare_all(processes, tq=2):
    print("\n" + "=" * 65)
    print("       SIDE-BY-SIDE CPU SCHEDULING ALGORITHM BENCHMARK")
    print("=" * 65)
    results = []

    # 1. FCFS
    p_fcfs, _ = solve_fcfs([dict(p) for p in processes])
    results.append(("FCFS", sum(p['tat'] for p in p_fcfs)/len(p_fcfs), sum(p['wt'] for p in p_fcfs)/len(p_fcfs)))

    # 2. SJF
    p_sjf, _ = solve_sjf_non_preemptive([dict(p) for p in processes])
    results.append(("SJF (Non-Preemptive)", sum(p['tat'] for p in p_sjf)/len(p_sjf), sum(p['wt'] for p in p_sjf)/len(p_sjf)))

    # 3. SRTF
    p_srtf, _ = solve_srtf([dict(p) for p in processes])
    results.append(("SRTF (Preemptive)", sum(p['tat'] for p in p_srtf)/len(p_srtf), sum(p['wt'] for p in p_srtf)/len(p_srtf)))

    # 4. Priority Non-Preemptive
    p_prio_np, _ = solve_priority_non_preemptive([dict(p) for p in processes])
    results.append(("Priority (Non-Preemptive)", sum(p['tat'] for p in p_prio_np)/len(p_prio_np), sum(p['wt'] for p in p_prio_np)/len(p_prio_np)))

    # 5. Priority Preemptive
    p_prio_p, _ = solve_priority_preemptive([dict(p) for p in processes])
    results.append(("Priority (Preemptive)", sum(p['tat'] for p in p_prio_p)/len(p_prio_p), sum(p['wt'] for p in p_prio_p)/len(p_prio_p)))

    # 6. Round Robin
    p_rr, _ = solve_round_robin([dict(p) for p in processes], tq)
    results.append((f"Round Robin (TQ={tq})", sum(p['tat'] for p in p_rr)/len(p_rr), sum(p['wt'] for p in p_rr)/len(p_rr)))

    print(f"{'Algorithm':<30} {'Average TAT':<18} {'Average WT':<18}")
    print("-" * 65)
    for name, avg_tat, avg_wt in results:
        print(f"{name:<30} {avg_tat:<18.2f} {avg_wt:<18.2f}")
    print("=" * 65)

    # Best performer
    best_wt = min(results, key=lambda x: x[2])
    print(f"Optimal for Waiting Time: {best_wt[0]} with Avg WT = {best_wt[2]:.2f}")

def main():
    procs = get_default_processes()
    while True:
        print("\n" + "=" * 55)
        print("          CPU SCHEDULING ALGORITHMS MENU")
        print("=" * 55)
        print("1. First-Come, First-Served (FCFS)")
        print("2. Shortest Job First (SJF Non-Preemptive)")
        print("3. Shortest Remaining Time First (SRTF Preemptive)")
        print("4. Priority Scheduling (Non-Preemptive)")
        print("5. Priority Scheduling (Preemptive)")
        print("6. Round Robin Scheduling (RR)")
        print("7. Compare All Algorithms on Current Dataset")
        print("8. Customize Input Processes")
        print("9. Exit")
        print("-" * 55)

        choice = input("Enter choice (1-9): ").strip()
        if choice == '1':
            p, g = solve_fcfs([dict(x) for x in procs])
            print_gantt_and_table("FCFS", p, g)
        elif choice == '2':
            p, g = solve_sjf_non_preemptive([dict(x) for x in procs])
            print_gantt_and_table("SJF Non-Preemptive", p, g)
        elif choice == '3':
            p, g = solve_srtf([dict(x) for x in procs])
            print_gantt_and_table("SRTF Preemptive", p, g)
        elif choice == '4':
            p, g = solve_priority_non_preemptive([dict(x) for x in procs])
            print_gantt_and_table("Priority Non-Preemptive", p, g)
        elif choice == '5':
            p, g = solve_priority_preemptive([dict(x) for x in procs])
            print_gantt_and_table("Priority Preemptive", p, g)
        elif choice == '6':
            tq_input = input("Enter Time Quantum (default 2): ").strip()
            tq = int(tq_input) if tq_input else 2
            p, g = solve_round_robin([dict(x) for x in procs], tq)
            print_gantt_and_table(f"Round Robin (TQ={tq})", p, g)
        elif choice == '7':
            tq_input = input("Enter Time Quantum for RR comparison (default 2): ").strip()
            tq = int(tq_input) if tq_input else 2
            compare_all(procs, tq)
        elif choice == '8':
            n = int(input("Enter number of processes: "))
            procs = []
            for i in range(n):
                print(f"\nProcess P{i + 1}:")
                at = int(input("  Arrival Time: "))
                bt = int(input("  Burst Time:   "))
                pr = int(input("  Priority (lower=higher): "))
                procs.append({'pid': i + 1, 'at': at, 'bt': bt, 'priority': pr})
            print("\nProcesses updated successfully!")
        elif choice == '9':
            print("Exiting CPU Scheduling Suite. Goodbye!")
            break
        else:
            print("Invalid selection. Please choose 1-9.")

if __name__ == "__main__":
    main()

