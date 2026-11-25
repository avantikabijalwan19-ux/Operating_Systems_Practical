/*
 * Practical: Priority Scheduling Algorithm (Preemptive)
 * Description:
 *     Preemptive variant of Priority Scheduling.
 *     At every clock unit, the CPU executes the arrived process having the highest priority
 *     (lowest numerical value). If a newly arrived process has higher priority, it preempts
 *     the currently executing process immediately.
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
    int priority;
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

int solve_priority_preemptive(Process procs[], int n, GanttSegment gantt[]) {
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

void display_results(Process procs[], int n, const GanttSegment gantt[], int gantt_count) {
    qsort(procs, n, sizeof(Process), cmp_pid);

    printf("\n===========================================================================\n");
    printf("%-6s %-10s %-10s %-10s %-12s %-10s %-10s\n",
           "PID", "Priority", "Arrival", "Burst", "Complete", "TAT", "WT");
    printf("===========================================================================\n");

    int total_tat = 0;
    int total_wt = 0;

    for (int i = 0; i < n; i++) {
        total_tat += procs[i].tat;
        total_wt += procs[i].wt;
        printf("P%-5d %-10d %-10d %-10d %-12d %-10d %-10d\n",
               procs[i].pid, procs[i].priority, procs[i].at, procs[i].bt,
               procs[i].ct, procs[i].tat, procs[i].wt);
    }

    printf("---------------------------------------------------------------------------\n");
    printf("Average Turnaround Time (TAT): %.2f\n", (double)total_tat / n);
    printf("Average Waiting Time (WT):    %.2f\n", (double)total_wt / n);
    printf("===========================================================================\n");

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
    printf("=================================================================\n");
    printf("        PRIORITY SCHEDULING (PREEMPTIVE)\n");
    printf("=================================================================\n");
    printf("Note: Lower priority number indicates higher priority (e.g. 1 > 2).\n");
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
            printf("  Priority (integer): ");
            scanf("%d", &procs[i].priority);
            printf("  Arrival Time:       ");
            scanf("%d", &procs[i].at);
            printf("  Burst Time:         ");
            scanf("%d", &procs[i].bt);
        }
    } else {
        Process defaults[] = {
            {1, 3, 0, 4, 0, 0, 0, 0},
            {2, 1, 1, 5, 0, 0, 0, 0},
            {3, 4, 2, 1, 0, 0, 0, 0},
            {4, 2, 3, 2, 0, 0, 0, 0}
        };
        n = 4;
        memcpy(procs, defaults, sizeof(defaults));
    }

    GanttSegment gantt[MAX_GANTT];
    int gantt_count = solve_priority_preemptive(procs, n, gantt);
    display_results(procs, n, gantt, gantt_count);

    return 0;
}

