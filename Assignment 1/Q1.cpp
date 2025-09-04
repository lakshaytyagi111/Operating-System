#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct ProcessQ1 {
    string name;
    int burst, priority, remaining, completion, waiting, turnaround, last_exec;
    bool done;
};

bool cmp_priority(const ProcessQ1 &a, const ProcessQ1 &b) {
    return a.priority < b.priority;
}
bool cmp_name(const ProcessQ1 &a, const ProcessQ1 &b) {
    return a.name < b.name;
}

int main() {
    vector<ProcessQ1> p = {{"P1",4,3,4,0,0,0,0,false},
                           {"P2",5,2,5,0,0,0,0,false},
                           {"P3",8,2,8,0,0,0,0,false},
                           {"P4",7,1,7,0,0,0,0,false},
                           {"P5",3,3,3,0,0,0,0,false}};
    int n = p.size(), time = 0, quantum = 2;
    vector<string> gantt;
    sort(p.begin(), p.end(), cmp_priority);

    vector<vector<int>> groups;
    int prev_pri = p[0].priority;
    vector<int> g;
    for(int i=0;i<n;++i) {
        if(p[i].priority != prev_pri && g.size()) {
            groups.push_back(g);
            g.clear();
        }
        g.push_back(i);
        prev_pri = p[i].priority;
    }
    if(g.size()) groups.push_back(g);

    for (auto &grp : groups) {
        queue<int> rr;
        for (int idx : grp) rr.push(idx);
        while(!rr.empty()) {
            int idx = rr.front(); rr.pop();
            if (p[idx].remaining <= 0) continue;
            int ex_time = min(quantum, p[idx].remaining);
            time += ex_time;
            p[idx].remaining -= ex_time;
            gantt.push_back(p[idx].name);
            p[idx].last_exec = time;
            if (p[idx].remaining > 0)
                rr.push(idx);
            else
                p[idx].completion = time;
        }
    }
    for(auto &pr : p){
        pr.turnaround = pr.completion;
        pr.waiting = pr.turnaround - pr.burst;
    }
    sort(p.begin(), p.end(), cmp_name);

    cout << "Order of Execution:\n";
    for (auto &g : gantt) cout << g << " ";
    cout << "\n";
    cout << "Completion time of P4: " << p[3].completion << '\n';
    cout << "Turnaround time of P2: " << p[1].turnaround << '\n';
    cout << "Waiting time of P3: " << p[2].waiting << '\n';
    double sum_tat=0;
    for(auto &pr:p){sum_tat+=pr.turnaround;}
    cout<<"Average Turnaround Time: "<<sum_tat/n<<"\n";
    double sum_wt=0;
    for(auto &pr:p){sum_wt+=pr.waiting;}
    cout<<"Average Waiting Time: "<<sum_wt/n<<"\n";
    cout << "Gantt Chart:\n|";
    for(auto &g:gantt) cout << " " << g << " |";
    cout << endl;
}
