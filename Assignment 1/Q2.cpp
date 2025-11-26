#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    char id;        
    int arrivalTime;
    int burstTime;
    int remainingTime;  
    int priority;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

// Reset computed fields before running each algorithm
void resetProcesses(vector<Process> &processes) {
    for (auto &p : processes) {
        p.remainingTime = p.burstTime;
        p.completionTime = 0;
        p.turnaroundTime = 0;
        p.waitingTime = 0;
    }
}

// Print results and averages
void printResults(const string &algoName, const vector<Process> &processes) {
    cout << "\n--- " << algoName << " ---\n";
    cout << "P  AT  BT  PR  CT  TAT  WT\n";
    float totalTAT = 0, totalWT = 0;
    for (const auto &p : processes) {
        cout << "P" << p.id << "  " << p.arrivalTime << "   " << p.burstTime 
             << "   " << p.priority << "   " << p.completionTime << "   " 
             << p.turnaroundTime << "    " << p.waitingTime << "\n";
        totalTAT += p.turnaroundTime;
        totalWT += p.waitingTime;
    }
    cout << "Average Turnaround Time: " << totalTAT / processes.size() << "\n";
    cout << "Average Waiting Time: " << totalWT / processes.size() << "\n";
}

// First-Come, First-Served Scheduling
void FCFS(vector<Process> processes) {
    resetProcesses(processes);
    int time = 0;
    int completed = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1;
        int earliestArrival = 1e9;
        for (int i = 0; i < n; i++) {
            if (processes[i].completionTime == 0 && processes[i].arrivalTime <= time && processes[i].arrivalTime < earliestArrival) {
                earliestArrival = processes[i].arrivalTime;
                idx = i;
            }
        }
        if (idx == -1) {
            // CPU idle, advance time
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (processes[i].completionTime == 0 && processes[i].arrivalTime < nextArrival) {
                    nextArrival = processes[i].arrivalTime;
                }
            }
            time = nextArrival;
            continue;
        }
        time += processes[idx].burstTime;
        processes[idx].completionTime = time;
        processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
        processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        completed++;
    }
    printResults("First-Come, First-Served (FCFS)", processes);
}

// Non-preemptive Shortest Job First Scheduling
void SJF(vector<Process> processes) {
    resetProcesses(processes);
    int time = 0;
    int completed = 0;
    int n = processes.size();

    cout << "\nGantt chart (SJF): ";
    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;
        for (int i = 0; i < n; i++) {
            if (processes[i].completionTime == 0 && processes[i].arrivalTime <= time && processes[i].burstTime < minBurst) {
                minBurst = processes[i].burstTime;
                idx = i;
            }
        }
        if (idx == -1) {
            // CPU idle, advance time
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (processes[i].completionTime == 0 && processes[i].arrivalTime < nextArrival) {
                    nextArrival = processes[i].arrivalTime;
                }
            }
            time = nextArrival;
            continue;
        }
        cout << "| P" << processes[idx].id << " ";
        time += processes[idx].burstTime;
        processes[idx].completionTime = time;
        processes[idx].turnaroundTime = time - processes[idx].arrivalTime;
        processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        completed++;
    }
    cout << "|\n";
    printResults("Shortest Job First (Non-preemptive)", processes);
}

// Round Robin Scheduling with given quantum
void RoundRobin(vector<Process> processes, int q) {
    resetProcesses(processes);
    int time = 0;
    int n = processes.size();
    int completed = 0;
    vector<bool> inQueue(n, false);
    vector<int> readyQueue;

    // Add processes arrived at time 0 to ready queue
    for (int i = 0; i < n; i++) {
        if (processes[i].arrivalTime == 0) {
            readyQueue.push_back(i);
            inQueue[i] = true;
        }
    }
    if (readyQueue.empty()) {
        // if no process arrived at time 0, jump to earliest arrival
        int earliestArrival = 1e9;
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime < earliestArrival) {
                earliestArrival = processes[i].arrivalTime;
                idx = i;
            }
        }
        time = earliestArrival;
        readyQueue.push_back(idx);
        inQueue[idx] = true;
    }

    cout << "\nGantt chart (Round Robin, quantum=" << q << "): ";
    while (completed < n) {
        if (readyQueue.empty()) {
            int nextArrival = 1e9;
            int idx = -1;
            for (int i = 0; i < n; i++) {
                if (processes[i].completionTime == 0 && !inQueue[i] && processes[i].arrivalTime < nextArrival) {
                    nextArrival = processes[i].arrivalTime;
                    idx = i;
                }
            }
            time = max(time, nextArrival);
            readyQueue.push_back(idx);
            inQueue[idx] = true;
        }
        int idx = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        if (processes[idx].completionTime != 0) continue;

        int execTime = (processes[idx].remainingTime > q) ? q : processes[idx].remainingTime;
        cout << "| P" << processes[idx].id << " ";
        time += execTime;
        processes[idx].remainingTime -= execTime;

        // Enqueue processes that arrived during this time
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && processes[i].arrivalTime <= time && processes[i].completionTime == 0) {
                readyQueue.push_back(i);
                inQueue[i] = true;
            }
        }

        // If process completed
        if (processes[idx].remainingTime == 0) {
            completed++;
            processes[idx].completionTime = time;
            processes[idx].turnaroundTime = time - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        } else {
            // Not finished: enqueue back
            readyQueue.push_back(idx);
        }
    }
    cout << "|\n";
    printResults("Round Robin (Time Quantum = " + to_string(q) + ")", processes);
}

// Non-preemptive Priority Scheduling (lower number = higher priority)
void PriorityScheduling(vector<Process> processes) {
    resetProcesses(processes);
    int time = 0;
    int completed = 0;
    int n = processes.size();

    cout << "\nGantt chart (Priority Scheduling): ";
    while (completed < n) {
        int idx = -1;
        int highestPriority = 1e9;
        for (int i = 0; i < n; i++) {
            if (processes[i].completionTime == 0 && processes[i].arrivalTime <= time &&
                processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }
        if (idx == -1) {
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (processes[i].completionTime == 0 && processes[i].arrivalTime < nextArrival) {
                    nextArrival = processes[i].arrivalTime;
                }
            }
            time = nextArrival;
            continue;
        }
        cout << "| P" << processes[idx].id << " ";
        time += processes[idx].burstTime;
        processes[idx].completionTime = time;
        processes[idx].turnaroundTime = time - processes[idx].arrivalTime;
        processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        completed++;
    }
    cout << "|\n";
    printResults("Priority Scheduling (Non-preemptive)", processes);
}

int main() {
    // Input processes as per your data
    vector<Process> procs = {
        {'1', 0, 8, 0, 2, 0, 0, 0},
        {'2', 1, 4, 0, 1, 0, 0, 0},
        {'3', 2, 9, 0, 3, 0, 0, 0},
        {'4', 3, 5, 0, 2, 0, 0, 0},
        {'5', 4, 2, 0, 4, 0, 0, 0},
        {'6', 5, 7, 0, 1, 0, 0, 0}
    };

    cout << "CPU Scheduling Algorithms Simulation\n";

    FCFS(procs);
    SJF(procs);
    RoundRobin(procs, 3);
    RoundRobin(procs, 10);
    PriorityScheduling(procs);

    return 0;
}
