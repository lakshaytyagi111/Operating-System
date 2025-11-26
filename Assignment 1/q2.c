#include <stdio.h>

#define N 6         // number of processes
#define Q1 3        // RR time quantum 1
#define Q2 10       // RR time quantum 2

// Structure to hold all process data
typedef struct {
    char id;        // process name: '1', '2', ...
    int at;         // arrival time
    int bt;         // burst time (original)
    int bt_rem;     // remaining burst (for RR)
    int pr;         // priority (lower = higher)
    int ct;         // completion time
    int tat;        // turnaround time
    int wt;         // waiting time
} Process;

// Helper: reset computed fields to run a fresh algorithm
void reset_for_algo(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].bt_rem = p[i].bt;   // remaining = full burst
        p[i].ct = p[i].tat = p[i].wt = 0;
    }
}

// Helper: print per‑process results and averages
void print_results(const char *title, Process p[], int n) {
    float avg_tat = 0.0, avg_wt = 0.0;

    printf("\n\n===== %s =====\n", title);
    printf("P  AT  BT  PR  CT  TAT  WT\n");
    for (int i = 0; i < n; i++) {
        printf("P%c  %2d  %2d  %2d  %2d  %3d  %3d\n",
               p[i].id, p[i].at, p[i].bt, p[i].pr,
               p[i].ct, p[i].tat, p[i].wt);
        avg_tat += p[i].tat;
        avg_wt  += p[i].wt;
    }
    avg_tat /= n;
    avg_wt  /= n;
    printf("Average TAT = %.2f, Average WT = %.2f\n", avg_tat, avg_wt);
}

/* ---------------- FCFS (First-Come, First-Served) ----------------
   - Use arrival order.
   - When CPU is free, pick the process with smallest arrival time
     among those not yet completed and already arrived.
   - Non‑preemptive: run each process to completion.
*/
void fcfs(Process orig[], int n) {
    Process p[N];
    for (int i = 0; i < n; i++) p[i] = orig[i];
    reset_for_algo(p, n);

    int completed = 0;
    int time = 0;

    while (completed < n) {
        int idx = -1;
        int earliest_at = 1e9;

        // Choose the not‑completed process with earliest arrival
        // that has already arrived (at <= time).
        for (int i = 0; i < n; i++) {
            if (p[i].ct == 0 && p[i].at <= time && p[i].at < earliest_at) {
                earliest_at = p[i].at;
                idx = i;
            }
        }

        // If no process has arrived yet, advance time to next arrival
        if (idx == -1) {
            int next_at = 1e9;
            for (int i = 0; i < n; i++) {
                if (p[i].ct == 0 && p[i].at < next_at)
                    next_at = p[i].at;
            }
            time = next_at;
            continue;
        }

        // Run process p[idx] to completion
        time += p[idx].bt;
        p[idx].ct  = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt  = p[idx].tat - p[idx].bt;
        completed++;
    }

    print_results("FCFS", p, n);
}

/* ---------------- Non‑preemptive SJF (Shortest Job First) ----------------
   - At each selection point, pick the process with smallest burst time
     among those that have arrived and are not completed.
   - Once selected, run to completion.
*/
void sjf_np(Process orig[], int n) {
    Process p[N];
    for (int i = 0; i < n; i++) p[i] = orig[i];
    reset_for_algo(p, n);

    int completed = 0;
    int time = 0;

    printf("\nGantt chart (SJF NP): ");

    while (completed < n) {
        int idx = -1;
        int min_bt = 1e9;

        // Find shortest burst among arrived, not completed
        for (int i = 0; i < n; i++) {
            if (p[i].ct == 0 && p[i].at <= time && p[i].bt < min_bt) {
                min_bt = p[i].bt;
                idx = i;
            }
        }

        // If nothing has arrived, jump to next arrival
        if (idx == -1) {
            int next_at = 1e9;
            for (int i = 0; i < n; i++) {
                if (p[i].ct == 0 && p[i].at < next_at)
                    next_at = p[i].at;
            }
            time = next_at;
            continue;
        }

        // "Run" this process
        printf("| P%c ", p[idx].id);
        time += p[idx].bt;
        p[idx].ct  = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt  = p[idx].tat - p[idx].bt;
        completed++;
    }
    printf("|\n");

    print_results("SJF (Non-preemptive)", p, n);
}

/* ---------------- Round Robin (RR) ----------------
   - Quantum q.
   - Maintain ready queue; run each process at most q ms, then move to end.
   - Arrival times considered: a process can only enter ready queue once arrived.
*/
void rr(Process orig[], int n, int q) {
    Process p[N];
    for (int i = 0; i < n; i++) p[i] = orig[i];
    reset_for_algo(p, n);

    int completed = 0;
    int time = 0;
    int done[N] = {0};    // 1 if process finished
    int ready[N];         // circular queue of indices
    int front = 0, rear = 0;

    printf("\nGantt chart (RR, q=%d): ", q);

    // Helper lambda‑like: enqueue process index
    #define ENQ(x) do { ready[rear] = (x); rear = (rear + 1) % N; } while(0)
    #define DEQ()  (front = (front + 1) % N)
    #define EMPTY() (front == rear)

    // Initially, bring any process that arrives at time 0
    int started[N] = {0};
    for (int i = 0; i < n; i++) {
        if (p[i].at == 0) {
            ENQ(i);
            started[i] = 1;
        }
    }
    if (EMPTY()) {
        // If nothing at time 0, move time to earliest arrival
        int earliest = 1e9, idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].at < earliest) {
                earliest = p[i].at; idx = i;
            }
        }
        time = earliest;
        ENQ(idx);
        started[idx] = 1;
    }

    while (completed < n) {
        // If ready queue empty, jump to next arriving process
        if (EMPTY()) {
            int next_at = 1e9, idx = -1;
            for (int i = 0; i < n; i++) {
                if (!done[i] && !started[i] && p[i].at < next_at) {
                    next_at = p[i].at; idx = i;
                }
            }
            if (idx == -1) break; // should not happen here
            if (time < p[idx].at) time = p[idx].at;
            ENQ(idx);
            started[idx] = 1;
        }

        int idx = ready[front];
        DEQ();

        if (done[idx]) continue;   // skip if already finished

        // How much to run in this slice
        int slice = (p[idx].bt_rem > q) ? q : p[idx].bt_rem;

        printf("| P%c ", p[idx].id);

        time += slice;
        p[idx].bt_rem -= slice;

        // Enqueue any processes that arrived during this time interval
        for (int i = 0; i < n; i++) {
            if (!started[i] && p[i].at <= time) {
                ENQ(i);
                started[i] = 1;
            }
        }

        // If this process finished now, record its times
        if (p[idx].bt_rem == 0) {
            done[idx] = 1;
            completed++;
            p[idx].ct  = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
        } else {
            // Still has remaining time, go to back of queue
            ENQ(idx);
        }
    }
    printf("|\n");

    char title[64];
    sprintf(title, "Round Robin (q = %d)", q);
    print_results(title, p, n);
}

/* ---------------- Non‑preemptive Priority Scheduling ----------------
   - At each selection point, pick process with highest priority
     (smallest priority number) among arrived and not completed.
   - Once selected, run to completion.
*/
void priority_np(Process orig[], int n) {
    Process p[N];
    for (int i = 0; i < n; i++) p[i] = orig[i];
    reset_for_algo(p, n);

    int completed = 0;
    int time = 0;

    printf("\nGantt chart (Priority NP): ");

    while (completed < n) {
        int idx = -1;
        int best_pr = 1e9;

        // Among arrived, not completed, pick lowest priority number
        for (int i = 0; i < n; i++) {
            if (p[i].ct == 0 && p[i].at <= time && p[i].pr < best_pr) {
                best_pr = p[i].pr;
                idx = i;
            }
        }

        if (idx == -1) {
            // Nothing arrived yet, jump to next arrival
            int next_at = 1e9;
            for (int i = 0; i < n; i++) {
                if (p[i].ct == 0 && p[i].at < next_at)
                    next_at = p[i].at;
            }
            time = next_at;
            continue;
        }

        printf("| P%c ", p[idx].id);
        time += p[idx].bt;
        p[idx].ct  = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt  = p[idx].tat - p[idx].bt;
        completed++;
    }
    printf("|\n");

    print_results("Priority (Non-preemptive)", p, n);
}

int main() {
    // Given processes:
    // P1 0 8 2
    // P2 1 4 1
    // P3 2 9 3
    // P4 3 5 2
    // P5 4 2 4
    // P6 5 7 1

    Process procs[N] = {
        {'1', 0, 8, 0, 2, 0,0,0},
        {'2', 1, 4, 0, 1, 0,0,0},
        {'3', 2, 9, 0, 3, 0,0,0},
        {'4', 3, 5, 0, 2, 0,0,0},
        {'5', 4, 2, 0, 4, 0,0,0},
        {'6', 5, 7, 0, 1, 0,0,0}
    };

    printf("CPU Scheduling Simulation for 6 Processes\n");

    fcfs(procs, N);          // (a) FCFS
    sjf_np(procs, N);        // (b) SJF non‑preemptive
    rr(procs, N, Q1);        // (c) RR, q = 3
    rr(procs, N, Q2);        // (c) RR, q = 10
    priority_np(procs, N);   // (d) Priority non‑preemptive

    return 0;
}
