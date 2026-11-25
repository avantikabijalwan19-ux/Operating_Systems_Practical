/*
 * Practical: Interactive Menu-Driven CPU Scheduling Suite and Comparative Benchmark
 * Description:
 *     A unified CLI dashboard integrating FCFS, SJF, SRTF, Priority (Non-Preemptive & Preemptive),
 *     and Round Robin. Allows running individual algorithms or generating a side-by-side
 *     performance comparison (Average TAT & Average WT).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PROCESSES 100
#define MAX_GANTT 1000
#define IDLE_PID -1

typedef struct {
    int pid;
    int at;
    int bt;
    int priority;
    int ct;
    int tat;
    int wt;
    int remaining_bt;
    int completed;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} GanttSegment;

static int cmp_arrival_pid(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    if (p1->at != p2->at) return p1->at - p2->at;
    return p1->pid - p2->pid;
}

static int cmp_pid(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->pid - p2->pid;
}

static void append_gantt_unit(GanttSegment gantt[], int *gantt_count, int pid, int time) {
    if (*gantt_count > 0 &&
        gantt[*gantt_count - 1].pid == pid &&
        gantt[*gantt_count - 1].end == time) {
        gantt[*gantt_count - 1].end = time + 1;
    } else if (*gantt_count < MAX_GANTT) {
        gantt[*gantt_count] = (GanttSegment){pid, time, time + 1};
        (*gantt_count)++;
    }
}

static int solve_fcfs(Process procs[], int n, GanttSegment gantt[]) {
    qsort(procs, n, sizeof(Process), cmp_arrival_pid);
    int current_time = 0, gantt_count = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < procs[i].at) {
            gantt[gantt_count++] = (GanttSegment){IDLE_PID, current_time, procs[i].at};
            current_time = procs[i].at;
        }
        int start = current_time;
        current_time += procs[i].bt;
        procs[i].ct = current_time;
        procs[i].tat = procs[i].ct - procs[i].at;
        procs[i].wt = procs[i].tat - procs[i].bt;
        gantt[gantt_count++] = (GanttSegment){procs[i].pid, start, current_time};
    }
    return gantt_count;
}

static int solve_sjf_non_preemptive(Process procs[], int n, GanttSegment gantt[]) {
    int completed = 0, current_time = 0, gantt_count = 0;
    for (int i = 0; i < n; i++) procs[i].completed = 0;

    while (completed < n) {
        int chosen = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].at <= current_time) {
                if (chosen == -1 ||
                    procs[i].bt < procs[chosen].bt ||
                    (procs[i].bt == procs[chosen].bt && procs[i].at < procs[chosen].at) ||
                    (procs[i].bt == procs[chosen].bt && procs[i].at == procs[chosen].at &&
                     procs[i].pid < procs[chosen].pid)) {
                    chosen = i;
                }
            }
        }
        if (chosen == -1) {
            int next_at = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!procs[i].completed && procs[i].at < next_at) next_at = procs[i].at;
            }
            gantt[gantt_count++] = (GanttSegment){IDLE_PID, current_time, next_at};
            current_time = next_at;
            continue;
        }
        int start = current_time;
        current_time += procs[chosen].bt;
        procs[chosen].ct = current_time;
        procs[chosen].tat = procs[chosen].ct - procs[chosen].at;
        procs[chosen].wt = procs[chosen].tat - procs[chosen].bt;
        procs[chosen].completed = 1;
        completed++;
        gantt[gantt_count++] = (GanttSegment){procs[chosen].pid, start, current_time};
    }
    return gantt_count;
}

static int solve_srtf(Process procs[], int n, GanttSegment gantt[]) {
    for (int i = 0; i < n; i++) procs[i].remaining_bt = procs[i].bt;
    int current_time = 0, completed = 0, gantt_count = 0;

    while (completed < n) {
        int chosen = -1;
        for (int i = 0; i < n; i++) {
            if (procs[i].at <= current_time && procs[i].remaining_bt > 0) {
                if (chosen == -1 ||
                    procs[i].remaining_bt < procs[chosen].remaining_bt ||
                    (procs[i].remaining_bt == procs[chosen].remaining_bt && procs[i].at < procs[chosen].at) ||
                    (procs[i].remaining_bt == procs[chosen].remaining_bt && procs[i].at == procs[chosen].at &&
                     procs[i].pid < procs[chosen].pid)) {
                    chosen = i;
                }
            }
        }
        if (chosen == -1) {
            append_gantt_unit(gantt, &gantt_count, IDLE_PID, current_time);
            current_time++;
            continue;
        }
        append_gantt_unit(gantt, &gantt_count, procs[chosen].pid, current_time);
        procs[chosen].remaining_bt--;
        current_time++;
        if (procs[chosen].remaining_bt == 0) {
            completed++;
            procs[chosen].ct = current_time;
            procs[chosen].tat = procs[chosen].ct - procs[chosen].at;
            procs[chosen].wt = procs[chosen].tat - procs[chosen].bt;
        }
    }
    return gantt_count;
}

static int solve_priority_non_preemptive(Process procs[], int n, GanttSegment gantt[]) {
    int completed = 0, current_time = 0, gantt_count = 0;
    for (int i = 0; i < n; i++) procs[i].completed = 0;

    while (completed < n) {
        int chosen = -1;
        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].at <= current_time) {
                if (chosen == -1 ||
                    procs[i].priority < procs[chosen].priority ||
                    (procs[i].priority == procs[chosen].priority && procs[i].at < procs[chosen].at) ||
                    (procs[i].priority == procs[chosen].priority && procs[i].at == procs[chosen].at &&
                     procs[i].pid < procs[chosen].pid)) {
                    chosen = i;
                }
            }
        }
        if (chosen == -1) {
            int next_at = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!procs[i].completed && procs[i].at < next_at) next_at = procs[i].at;
            }
            gantt[gantt_count++] = (GanttSegment){IDLE_PID, current_time, next_at};
            current_time = next_at;
            continue;
        }
        int start = current_time;
        current_time += procs[chosen].bt;
        procs[chosen].ct = current_time;
        procs[chosen].tat = procs[chosen].ct - procs[chosen].at;
        procs[chosen].wt = procs[chosen].tat - procs[chosen].bt;
        procs[chosen].completed = 1;
        completed++;
        gantt[gantt_count++] = (GanttSegment){procs[chosen].pid, start, current_time};
    }
    return gantt_count;
}

static int solve_priority_preemptive(Process procs[], int n, GanttSegment gantt[]) {
    for (int i = 0; i < n; i++) procs[i].remaining_bt = procs[i].bt;
    int current_time = 0, completed = 0, gantt_count = 0;

    while (completed < n) {
        int chosen = -1;
        for (int i = 0; i < n; i++) {
            if (procs[i].at <= current_time && procs[i].remaining_bt > 0) {
                if (chosen == -1 ||
                    procs[i].priority < procs[chosen].priority ||
                    (procs[i].priority == procs[chosen].priority && procs[i].at < procs[chosen].at) ||
                    (procs[i].priority == procs[chosen].priority && procs[i].at == procs[chosen].at &&
                     procs[i].pid < procs[chosen].pid)) {
                    chosen = i;
                }
            }
        }
        if (chosen == -1) {
            append_gantt_unit(gantt, &gantt_count, IDLE_PID, current_time);
            current_time++;
            continue;
        }
        append_gantt_unit(gantt, &gantt_count, procs[chosen].pid, current_time);
        procs[chosen].remaining_bt--;
        current_time++;
        if (procs[chosen].remaining_bt == 0) {
            completed++;
            procs[chosen].ct = current_time;
            procs[chosen].tat = procs[chosen].ct - procs[chosen].at;
            procs[chosen].wt = procs[chosen].tat - procs[chosen].bt;
        }
    }
    return gantt_count;
}

static int solve_round_robin(Process procs[], int n, int tq, GanttSegment gantt[]) {
    qsort(procs, n, sizeof(Process), cmp_arrival_pid);
    int in_queue[MAX_PROCESSES] = {0};
    for (int i = 0; i < n; i++) procs[i].remaining_bt = procs[i].bt;

    int queue[MAX_PROCESSES];
    int front = 0, rear = 0, q_count = 0;
    int current_time = 0, completed = 0, idx = 0, gantt_count = 0;

    while (idx < n && procs[idx].at <= current_time) {
        queue[rear] = idx;
        rear = (rear + 1) % MAX_PROCESSES;
        q_count++;
        in_queue[idx] = 1;
        idx++;
    }

    while (completed < n) {
        if (q_count == 0) {
            if (idx < n) {
                int next_at = procs[idx].at;
                gantt[gantt_count++] = (GanttSegment){IDLE_PID, current_time, next_at};
                current_time = next_at;
                while (idx < n && procs[idx].at <= current_time) {
                    queue[rear] = idx;
                    rear = (rear + 1) % MAX_PROCESSES;
                    q_count++;
                    in_queue[idx] = 1;
                    idx++;
                }
            }
            continue;
        }

        int curr = queue[front];
        front = (front + 1) % MAX_PROCESSES;
        q_count--;

        int start = current_time;
        int exec_time = procs[curr].remaining_bt < tq ? procs[curr].remaining_bt : tq;
        current_time += exec_time;
        procs[curr].remaining_bt -= exec_time;
        gantt[gantt_count++] = (GanttSegment){procs[curr].pid, start, current_time};

        while (idx < n && procs[idx].at <= current_time) {
            if (!in_queue[idx]) {
                queue[rear] = idx;
                rear = (rear + 1) % MAX_PROCESSES;
                q_count++;
                in_queue[idx] = 1;
            }
            idx++;
        }

        if (procs[curr].remaining_bt > 0) {
            queue[rear] = curr;
            rear = (rear + 1) % MAX_PROCESSES;
            q_count++;
        } else {
            completed++;
            procs[curr].ct = current_time;
            procs[curr].tat = procs[curr].ct - procs[curr].at;
            procs[curr].wt = procs[curr].tat - procs[curr].bt;
        }
    }
    return gantt_count;
}

static void compute_averages(const Process procs[], int n, double *avg_tat, double *avg_wt) {
    int total_tat = 0, total_wt = 0;
    for (int i = 0; i < n; i++) {
        total_tat += procs[i].tat;
        total_wt += procs[i].wt;
    }
    *avg_tat = (double)total_tat / n;
    *avg_wt = (double)total_wt / n;
}

static void print_gantt_and_table(const char *algo_name, Process procs[], int n,
                                  const GanttSegment gantt[], int gantt_count) {
    qsort(procs, n, sizeof(Process), cmp_pid);

    printf("\n======================================================================\n");
    printf("Algorithm: %s\n", algo_name);
    printf("======================================================================\n");
    printf("%-8s %-10s %-10s %-12s %-10s %-10s\n",
           "PID", "Arrival", "Burst", "Complete", "TAT", "WT");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%-7d %-10d %-10d %-12d %-10d %-10d\n",
               procs[i].pid, procs[i].at, procs[i].bt,
               procs[i].ct, procs[i].tat, procs[i].wt);
    }

    double avg_tat = 0.0, avg_wt = 0.0;
    compute_averages(procs, n, &avg_tat, &avg_wt);
    printf("----------------------------------------------------------------------\n");
    printf("Average Turnaround Time: %.2f\n", avg_tat);
    printf("Average Waiting Time:    %.2f\n", avg_wt);

    if (gantt_count == 0) return;

    printf("\nGantt Chart:\n");
    int bar_len = 1;
    for (int i = 0; i < gantt_count; i++) {
        char label[32];
        if (gantt[i].pid == IDLE_PID) snprintf(label, sizeof(label), " IDLE ");
        else snprintf(label, sizeof(label), " P%d ", gantt[i].pid);
        bar_len += (int)strlen(label) + 1;
    }

    for (int i = 0; i < bar_len; i++) putchar('-');
    putchar('\n');
    putchar('|');
    for (int i = 0; i < gantt_count; i++) {
        if (gantt[i].pid == IDLE_PID) printf(" IDLE |");
        else printf(" P%d |", gantt[i].pid);
    }
    putchar('\n');
    for (int i = 0; i < bar_len; i++) putchar('-');
    putchar('\n');

    printf("%d", gantt[0].start);
    for (int i = 0; i < gantt_count; i++) {
        char label[32];
        if (gantt[i].pid == IDLE_PID) snprintf(label, sizeof(label), " IDLE ");
        else snprintf(label, sizeof(label), " P%d ", gantt[i].pid);
        printf("%*d", (int)strlen(label) + 1, gantt[i].end);
    }
    putchar('\n');
}

static void compare_all(const Process base_procs[], int n, int tq) {
    printf("\n=================================================================\n");
    printf("       SIDE-BY-SIDE CPU SCHEDULING ALGORITHM BENCHMARK\n");
    printf("=================================================================\n");

    struct {
        char name[40];
        double avg_tat;
        double avg_wt;
    } results[6];

    Process work[MAX_PROCESSES];
    GanttSegment gantt[MAX_GANTT];

    memcpy(work, base_procs, n * sizeof(Process));
    solve_fcfs(work, n, gantt);
    strcpy(results[0].name, "FCFS");
    compute_averages(work, n, &results[0].avg_tat, &results[0].avg_wt);

    memcpy(work, base_procs, n * sizeof(Process));
    solve_sjf_non_preemptive(work, n, gantt);
    strcpy(results[1].name, "SJF (Non-Preemptive)");
    compute_averages(work, n, &results[1].avg_tat, &results[1].avg_wt);

    memcpy(work, base_procs, n * sizeof(Process));
    solve_srtf(work, n, gantt);
    strcpy(results[2].name, "SRTF (Preemptive)");
    compute_averages(work, n, &results[2].avg_tat, &results[2].avg_wt);

    memcpy(work, base_procs, n * sizeof(Process));
    solve_priority_non_preemptive(work, n, gantt);
    strcpy(results[3].name, "Priority (Non-Preemptive)");
    compute_averages(work, n, &results[3].avg_tat, &results[3].avg_wt);

    memcpy(work, base_procs, n * sizeof(Process));
    solve_priority_preemptive(work, n, gantt);
    strcpy(results[4].name, "Priority (Preemptive)");
    compute_averages(work, n, &results[4].avg_tat, &results[4].avg_wt);

    memcpy(work, base_procs, n * sizeof(Process));
    solve_round_robin(work, n, tq, gantt);
    snprintf(results[5].name, sizeof(results[5].name), "Round Robin (TQ=%d)", tq);
    compute_averages(work, n, &results[5].avg_tat, &results[5].avg_wt);

    printf("%-30s %-18s %-18s\n", "Algorithm", "Average TAT", "Average WT");
    printf("-----------------------------------------------------------------\n");
    int best_idx = 0;
    for (int i = 0; i < 6; i++) {
        printf("%-30s %-18.2f %-18.2f\n", results[i].name, results[i].avg_tat, results[i].avg_wt);
        if (results[i].avg_wt < results[best_idx].avg_wt) best_idx = i;
    }
    printf("=================================================================\n");
    printf("Optimal for Waiting Time: %s with Avg WT = %.2f\n",
           results[best_idx].name, results[best_idx].avg_wt);
}

static int read_int_or_default(const char *prompt, int default_val) {
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return default_val;
    if (buf[0] == '\n' || buf[0] == '\0') return default_val;
    char *endptr;
    long val = strtol(buf, &endptr, 10);
    if (endptr == buf) return default_val;
    return (int)val;
}

int main(void) {
    Process procs[MAX_PROCESSES] = {
        {1, 0, 7, 3, 0, 0, 0, 0, 0},
        {2, 2, 4, 1, 0, 0, 0, 0, 0},
        {3, 4, 1, 4, 0, 0, 0, 0, 0},
        {4, 5, 4, 2, 0, 0, 0, 0, 0}
    };
    int n = 4;

    char line[64];
    while (1) {
        printf("\n=======================================================\n");
        printf("          CPU SCHEDULING ALGORITHMS MENU\n");
        printf("=======================================================\n");
        printf("1. First-Come, First-Served (FCFS)\n");
        printf("2. Shortest Job First (SJF Non-Preemptive)\n");
        printf("3. Shortest Remaining Time First (SRTF Preemptive)\n");
        printf("4. Priority Scheduling (Non-Preemptive)\n");
        printf("5. Priority Scheduling (Preemptive)\n");
        printf("6. Round Robin Scheduling (RR)\n");
        printf("7. Compare All Algorithms on Current Dataset\n");
        printf("8. Customize Input Processes\n");
        printf("9. Exit\n");
        printf("-------------------------------------------------------\n");
        printf("Enter choice (1-9): ");

        if (!fgets(line, sizeof(line), stdin)) break;
        int choice = (int)strtol(line, NULL, 10);

        Process work[MAX_PROCESSES];
        GanttSegment gantt[MAX_GANTT];
        int gc = 0;

        if (choice == 1) {
            memcpy(work, procs, n * sizeof(Process));
            gc = solve_fcfs(work, n, gantt);
            print_gantt_and_table("FCFS", work, n, gantt, gc);
        } else if (choice == 2) {
            memcpy(work, procs, n * sizeof(Process));
            gc = solve_sjf_non_preemptive(work, n, gantt);
            print_gantt_and_table("SJF Non-Preemptive", work, n, gantt, gc);
        } else if (choice == 3) {
            memcpy(work, procs, n * sizeof(Process));
            gc = solve_srtf(work, n, gantt);
            print_gantt_and_table("SRTF Preemptive", work, n, gantt, gc);
        } else if (choice == 4) {
            memcpy(work, procs, n * sizeof(Process));
            gc = solve_priority_non_preemptive(work, n, gantt);
            print_gantt_and_table("Priority Non-Preemptive", work, n, gantt, gc);
        } else if (choice == 5) {
            memcpy(work, procs, n * sizeof(Process));
            gc = solve_priority_preemptive(work, n, gantt);
            print_gantt_and_table("Priority Preemptive", work, n, gantt, gc);
        } else if (choice == 6) {
            int tq = read_int_or_default("Enter Time Quantum (default 2): ", 2);
            if (tq <= 0) tq = 2;
            char title[64];
            snprintf(title, sizeof(title), "Round Robin (TQ=%d)", tq);
            memcpy(work, procs, n * sizeof(Process));
            gc = solve_round_robin(work, n, tq, gantt);
            print_gantt_and_table(title, work, n, gantt, gc);
        } else if (choice == 7) {
            int tq = read_int_or_default("Enter Time Quantum for RR comparison (default 2): ", 2);
            if (tq <= 0) tq = 2;
            compare_all(procs, n, tq);
        } else if (choice == 8) {
            int new_n = read_int_or_default("Enter number of processes: ", 0);
            if (new_n <= 0 || new_n > MAX_PROCESSES) {
                printf("Invalid number of processes (must be 1-%d).\n", MAX_PROCESSES);
                continue;
            }
            n = new_n;
            for (int i = 0; i < n; i++) {
                procs[i].pid = i + 1;
                printf("\nProcess P%d:\n", i + 1);
                procs[i].at = read_int_or_default("  Arrival Time: ", 0);
                procs[i].bt = read_int_or_default("  Burst Time:   ", 1);
                procs[i].priority = read_int_or_default("  Priority (lower=higher): ", 1);
            }
            printf("\nProcesses updated successfully!\n");
        } else if (choice == 9) {
            printf("Exiting CPU Scheduling Suite. Goodbye!\n");
            break;
        } else {
            printf("Invalid selection. Please choose 1-9.\n");
        }
    }

    return 0;
}

