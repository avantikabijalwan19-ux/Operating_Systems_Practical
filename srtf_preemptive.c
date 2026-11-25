/*
 * Practical: Shortest Remaining Time First (SRTF) CPU Scheduling Algorithm (Preemptive SJF)
 * Description:
 *     Preemptive variant of SJF. At every unit of time, the CPU is allocated to the
 *     process with the smallest remaining execution time. Preempts the running process
 *     if a newly arrived process requires less remaining time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROCESSES 100
#define MAX_GANTT 1000
#define IDLE_PID -1

typedef struct {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int remaining_bt;
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

int solve_srtf(Process procs[], int n, GanttSegment gantt[]) {
    for (int i = 0; i < n; i++) {
        procs[i].remaining_bt = procs[i].bt;
    }

    int current_time = 0;
    int completed = 0;
    int gantt_count = 0;

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
    printf("  SHORTEST REMAINING TIME FIRST (SRTF) PREEMPTIVE SCHEDULING\n");
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
            {1, 0, 8, 0, 0, 0, 0},
            {2, 1, 4, 0, 0, 0, 0},
            {3, 2, 2, 0, 0, 0, 0},
            {4, 3, 1, 0, 0, 0, 0},
            {5, 4, 3, 0, 0, 0, 0}
        };
        n = 5;
        memcpy(procs, defaults, sizeof(defaults));
    }

    GanttSegment gantt[MAX_GANTT];
    int gantt_count = solve_srtf(procs, n, gantt);
    display_results(procs, n, gantt, gantt_count);

    return 0;
}

