/*
 * Practical: Shortest Job First (SJF) CPU Scheduling Algorithm (Non-Preemptive)
 * Description:
 *     Selects the process with the smallest Burst Time (BT) among all arrived processes.
 *     Once assigned to the CPU, it runs to completion (non-preemptive).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_PROCESSES 100
#define MAX_GANTT 200
#define IDLE_PID -1

typedef struct {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int completed;
} Process;

typedef struct {
    int pid;
    int start;
    int end;
} GanttSegment;

static int cmp_pid(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->pid - p2->pid;
}

int solve_sjf_non_preemptive(Process procs[], int n, GanttSegment gantt[]) {
    int completed = 0;
    int current_time = 0;
    int gantt_count = 0;

    for (int i = 0; i < n; i++) {
        procs[i].completed = 0;
    }

    while (completed < n) {
        int chosen_idx = -1;

        for (int i = 0; i < n; i++) {
            if (!procs[i].completed && procs[i].at <= current_time) {
                if (chosen_idx == -1 ||
                    procs[i].bt < procs[chosen_idx].bt ||
                    (procs[i].bt == procs[chosen_idx].bt && procs[i].at < procs[chosen_idx].at) ||
                    (procs[i].bt == procs[chosen_idx].bt && procs[i].at == procs[chosen_idx].at &&
                     procs[i].pid < procs[chosen_idx].pid)) {
                    chosen_idx = i;
                }
            }
        }

        if (chosen_idx == -1) {
            int next_at = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!procs[i].completed && procs[i].at < next_at) {
                    next_at = procs[i].at;
                }
            }
            gantt[gantt_count++] = (GanttSegment){IDLE_PID, current_time, next_at};
            current_time = next_at;
            continue;
        }

        int start = current_time;
        current_time += procs[chosen_idx].bt;
        procs[chosen_idx].ct = current_time;
        procs[chosen_idx].tat = procs[chosen_idx].ct - procs[chosen_idx].at;
        procs[chosen_idx].wt = procs[chosen_idx].tat - procs[chosen_idx].bt;
        procs[chosen_idx].completed = 1;
        completed++;
        gantt[gantt_count++] = (GanttSegment){procs[chosen_idx].pid, start, current_time};
    }

    return gantt_count;
}

void display_results(Process procs[], int n, const GanttSegment gantt[], int gantt_count) {
    qsort(procs, n, sizeof(Process), cmp_pid);

    printf("\n=================================================================\n");
    printf("%-8s %-10s %-10s %-12s %-10s %-10s\n",
           "PID", "Arrival", "Burst", "Complete", "TAT", "WT");
    printf("=================================================================\n");

    int total_tat = 0;
    int total_wt = 0;

    for (int i = 0; i < n; i++) {
        total_tat += procs[i].tat;
        total_wt += procs[i].wt;
        printf("P%-7d %-10d %-10d %-12d %-10d %-10d\n",
               procs[i].pid, procs[i].at, procs[i].bt,
               procs[i].ct, procs[i].tat, procs[i].wt);
    }

    printf("-----------------------------------------------------------------\n");
    printf("Average Turnaround Time (TAT): %.2f\n", (double)total_tat / n);
    printf("Average Waiting Time (WT):    %.2f\n", (double)total_wt / n);
    printf("=================================================================\n");

    if (gantt_count == 0) return;

    printf("\nGantt Chart:\n");
    int bar_len = 1;
    for (int i = 0; i < gantt_count; i++) {
        char label[32];
        if (gantt[i].pid == IDLE_PID) {
            snprintf(label, sizeof(label), " IDLE ");
        } else {
            snprintf(label, sizeof(label), " P%d ", gantt[i].pid);
        }
        bar_len += (int)strlen(label) + 1;
    }

    for (int i = 0; i < bar_len; i++) putchar('-');
    putchar('\n');

    putchar('|');
    for (int i = 0; i < gantt_count; i++) {
        if (gantt[i].pid == IDLE_PID) {
            printf(" IDLE |");
        } else {
            printf(" P%d |", gantt[i].pid);
        }
    }
    putchar('\n');

    for (int i = 0; i < bar_len; i++) putchar('-');
    putchar('\n');

    printf("%d", gantt[0].start);
    for (int i = 0; i < gantt_count; i++) {
        char label[32];
        if (gantt[i].pid == IDLE_PID) {
            snprintf(label, sizeof(label), " IDLE ");
        } else {
            snprintf(label, sizeof(label), " P%d ", gantt[i].pid);
        }
        printf("%*d", (int)strlen(label) + 1, gantt[i].end);
    }
    printf("\n\n");
}

int main(void) {
    printf("============================================================\n");
    printf("    SHORTEST JOB FIRST (SJF) NON-PREEMPTIVE SCHEDULING\n");
    printf("============================================================\n");
    printf("Use default sample processes? (Y/n): ");

    char choice[16];
    if (!fgets(choice, sizeof(choice), stdin)) return 1;

    Process procs[MAX_PROCESSES];
    int n = 0;

    if (tolower((unsigned char)choice[0]) == 'n') {
        printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
        if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCESSES) {
            fprintf(stderr, "Invalid number of processes.\n");
            return 1;
        }
        for (int i = 0; i < n; i++) {
            procs[i].pid = i + 1;
            printf("\nProcess P%d:\n", i + 1);
            printf("  Arrival Time: ");
            scanf("%d", &procs[i].at);
            printf("  Burst Time:   ");
            scanf("%d", &procs[i].bt);
        }
    } else {
        Process defaults[] = {
            {1, 1, 7, 0, 0, 0, 0},
            {2, 2, 5, 0, 0, 0, 0},
            {3, 3, 1, 0, 0, 0, 0},
            {4, 4, 2, 0, 0, 0, 0},
            {5, 5, 8, 0, 0, 0, 0}
        };
        n = 5;
        memcpy(procs, defaults, sizeof(defaults));
    }

    GanttSegment gantt[MAX_GANTT];
    int gantt_count = solve_sjf_non_preemptive(procs, n, gantt);
    display_results(procs, n, gantt, gantt_count);

    return 0;
}

