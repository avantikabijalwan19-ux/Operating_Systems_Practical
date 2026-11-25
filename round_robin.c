/*
 * Practical: Round Robin (RR) CPU Scheduling Algorithm
 * Description:
 *     Processes are dispatched in a FIFO ready queue and allocated CPU time in fixed slices
 *     called Time Quantum (TQ). If a process does not complete within TQ, it is preempted
 *     and re-enqueued at the back of the ready queue.
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

int solve_round_robin(Process procs[], int n, int time_quantum, GanttSegment gantt[]) {
    qsort(procs, n, sizeof(Process), cmp_arrival_pid);

    int in_queue[MAX_PROCESSES] = {0};
    for (int i = 0; i < n; i++) {
        procs[i].remaining_bt = procs[i].bt;
    }

    int queue[MAX_PROCESSES];
    int front = 0, rear = 0, q_count = 0;

    int current_time = 0;
    int completed = 0;
    int idx = 0;
    int gantt_count = 0;

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
        int exec_time = procs[curr].remaining_bt < time_quantum ? procs[curr].remaining_bt : time_quantum;
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

void display_results(Process procs[], int n, const GanttSegment gantt[], int gantt_count, int time_quantum) {
    qsort(procs, n, sizeof(Process), cmp_pid);

    printf("\n=================================================================\n");
    printf("ROUND ROBIN SCHEDULING (Time Quantum = %d)\n", time_quantum);
    printf("=================================================================\n");
    printf("%-8s %-10s %-10s %-12s %-10s %-10s\n",
           "PID", "Arrival", "Burst", "Complete", "TAT", "WT");
    printf("-----------------------------------------------------------------\n");

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
    printf("       ROUND ROBIN (RR) CPU SCHEDULING\n");
    printf("============================================================\n");
    printf("Use default sample processes? (Y/n): ");

    char choice[16];
    if (!fgets(choice, sizeof(choice), stdin)) return 1;

    Process procs[MAX_PROCESSES];
    int n = 0;
    int tq = 2;

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
        printf("\nEnter Time Quantum: ");
        if (scanf("%d", &tq) != 1 || tq <= 0) {
            fprintf(stderr, "Invalid time quantum.\n");
            return 1;
        }
    } else {
        Process defaults[] = {
            {1, 0, 5, 0, 0, 0, 0},
            {2, 1, 4, 0, 0, 0, 0},
            {3, 2, 2, 0, 0, 0, 0},
            {4, 4, 1, 0, 0, 0, 0}
        };
        n = 4;
        memcpy(procs, defaults, sizeof(defaults));
        tq = 2;
    }

    GanttSegment gantt[MAX_GANTT];
    int gantt_count = solve_round_robin(procs, n, tq, gantt);
    display_results(procs, n, gantt, gantt_count, tq);

    return 0;
}

