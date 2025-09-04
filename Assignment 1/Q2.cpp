#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <bits/stdc++.h>
using namespace std;

struct ProcessQ2 {
    string name;
    int arrival, burst, priority, remaining, start, completion, waiting, turnaround;
    bool done;
};

void fcfs(vector<ProcessQ2> p) {
    sort(p.begin(), p.end(), [](const auto &a, const auto &b){return a.arrival<b.arrival;});
    int time = 0;
    for(auto &pr:p){
        time = max(time, pr.arrival);
        pr.start = time;
        pr.completion = pr.start + pr.burst;
        pr.turnaround = pr.completion - pr.arrival;
        pr.waiting = pr.start - pr.arrival;
        time += pr.burst;
    }
    cout<<"FCFS Results\n";
    for(auto &pr:p){
        cout<<pr.name<<" CT:"<<pr.completion<<" TAT:"<<pr.turnaround<<" WT:"<<pr.waiting<<"\n";
    }
    double avg_tat=0, avg_wt=0;
    for(auto &pr:p){ avg_tat+=pr.turnaround; avg_wt+=pr.waiting;}
    cout<<"Average Turnaround: "<<avg_tat/p.size()<<"\n";
    cout<<"Average Waiting: "<<avg_wt/p.size()<<"\n";
}

void sjf_nonpre(vector<ProcessQ2> p) {
    int n = p.size(), done = 0, time = 0;
    vector<bool> completed(n, false);
    cout<<"SJF Non-Preemptive Gantt Chart: ";
    while(done < n){
        int idx = -1, minb = INT_MAX;
        for(int i=0;i<n;i++){
            if(!completed[i] && p[i].arrival<=time && p[i].burst<minb){
                minb=p[i].burst; idx=i;
            }
        }
        if(idx==-1){time++; continue;}
        p[idx].start = time;
        p[idx].completion = time + p[idx].burst;
        p[idx].turnaround = p[idx].completion-p[idx].arrival;
        p[idx].waiting = p[idx].start-p[idx].arrival;
        time += p[idx].burst;
        completed[idx]=true;done++;
        cout<<p[idx].name<<" ";
    }
    cout<<"\n";
    double avg_tat=0, avg_wt=0;
    for(auto &pr:p){ avg_tat+=pr.turnaround; avg_wt+=pr.waiting;}
    cout<<"Average Turnaround: "<<avg_tat/p.size()<<"\n";
    cout<<"Average Waiting: "<<avg_wt/p.size()<<"\n";
}

void round_robin(vector<ProcessQ2> p, int quantum) {
    int n = p.size(), time = 0, done = 0;
    vector<bool> completed(n, false);
    vector<int> comp(n, 0), tat(n, 0), wt(n, 0), remaining(n, 0);
    for (int i = 0; i < n; i++) remaining[i] = p[i].burst;
    queue<int> q;
    vector<string> gantt;
    int last_arv = 0;

    set<int> inQueue;

    while (done < n) {
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && remaining[i] > 0 && inQueue.find(i) == inQueue.end()) {
                q.push(i);
                inQueue.insert(i); 
            }
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int idx = q.front();
        q.pop();
        inQueue.erase(idx); 
        if (p[idx].arrival > time) {
            time = p[idx].arrival;
        }

        int exec = min(quantum, remaining[idx]);
        time += exec;
        gantt.push_back(p[idx].name);
        remaining[idx] -= exec;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival > last_arv && p[i].arrival <= time && remaining[i] > 0) {
                if (inQueue.find(i) == inQueue.end()) {
                    q.push(i);
                    inQueue.insert(i);
                }
            }
        }

        last_arv = time;

        if (remaining[idx] == 0 && comp[idx] == 0) {
            comp[idx] = time;
            done++;
        } else if (remaining[idx] > 0) {
            q.push(idx);
            inQueue.insert(idx);  
        }
    }

    // Print Gantt chart
    cout << "RR Gantt (Q=" << quantum << ")\n| ";
    for (auto &g : gantt) {
        cout << g << " | ";
    }
    cout << "\n";

    // Calculate TAT and WT
    for (int i = 0; i < n; i++) {
        tat[i] = comp[i] - p[i].arrival;
        wt[i] = tat[i] - p[i].burst;
    }

    // Calculate averages
    double avg_tat = 0, avg_wt = 0;
    for (int i = 0; i < n; i++) {
        avg_tat += tat[i];
        avg_wt += wt[i];
        cout << p[i].name << " CT:" << comp[i] << " TAT:" << tat[i] << " WT:" << wt[i] << "\n";
    }
    cout << "Average Turnaround: " << avg_tat / n << "\n";
    cout << "Average Waiting: " << avg_wt / n << "\n";
}


void priority_np(vector<ProcessQ2> p) {
    int n=p.size(), time=0, done=0;
    vector<bool> completed(n,false);
    cout<<"Priority Non-Preemptive Gantt Chart: ";
    while(done<n){
        int idx=-1, bestp=INT_MAX;
        for(int i=0;i<n;i++){
            if(!completed[i] && p[i].arrival<=time && p[i].priority<bestp){
                bestp=p[i].priority; idx=i;
            }
        }
        if(idx==-1){time++; continue;}
        p[idx].start = time;
        p[idx].completion = time + p[idx].burst;
        p[idx].turnaround = p[idx].completion-p[idx].arrival;
        p[idx].waiting = p[idx].start-p[idx].arrival;
        time += p[idx].burst;
        completed[idx]=true;done++;
        cout<<p[idx].name<<" ";
    }
    cout<<"\n";
    double avg_tat=0, avg_wt=0;
    for(auto &pr:p){ avg_tat+=pr.turnaround; avg_wt+=pr.waiting;}
    cout<<"Average Turnaround: "<<avg_tat/n<<"\n";
    cout<<"Average Waiting: "<<avg_wt/n<<"\n";
}

int main(){
    vector<ProcessQ2> p2 = {{"P1",0,8,2,8,0,0,0,0,false},
                            {"P2",1,4,1,4,0,0,0,0,false},
                            {"P3",2,9,3,9,0,0,0,0,false},
                            {"P4",3,5,2,5,0,0,0,0,false},
                            {"P5",4,2,4,2,0,0,0,0,false},
                            {"P6",5,7,1,7,0,0,0,0,false}};
    fcfs(p2);
    cout<<"\n";
    sjf_nonpre(p2);
    cout<<"\n";
    round_robin(p2,3);
    cout<<"\n";
    round_robin(p2,10);
    cout<<"\n";
    priority_np(p2);
}
