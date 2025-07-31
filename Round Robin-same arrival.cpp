#include <iostream>
#include <vector>
using namespace std;

void findWaitTime(vector<int> processes, int n, vector<int>& burst_time, vector<int>& wt_time, int quantum){
    vector<int> bt_time = burst_time;
    int t = 0;

    while(1){
        bool done = true;
        for (int i = 0; i < n; i++){
            if (bt_time[i] > 0){
                done = false;
                if (bt_time[i] > quantum){
                    t += quantum;
                    bt_time[i] -= quantum;
                } else {
                    t += bt_time[i];
                    wt_time[i] = t-burst_time[i];
                    bt_time[i] = 0;
                }
        }

    }
    if (done == true){
        break;
    }
}
}

void findTaTime(vector<int> processes, int n, vector<int>& burst_time, vector<int>& wt_time, vector<int>& tat){
    for (int i = 0; i < n; i++){
        tat[i] = burst_time[i] + wt_time[i];
    }

}

void findAvgTime(vector<int> processes, int n, vector<int>& burst_time, int quantum){
    vector<int> wt_time(n,0);
    vector<int> tat(n,0);
    int total_wt = 0;
    int total_tat = 0;

    findWaitTime(processes, n, burst_time, wt_time, quantum);

    findTaTime(processes, n, burst_time, wt_time, tat);

    for (int i = 0; i < n; i++)
    {
        total_wt = total_wt + wt_time[i];
        total_tat = total_tat + tat[i];
        cout << " " << i + 1 << "\t\t" << burst_time[i] << "\t " << wt_time[i] << "\t\t " << tat[i] << endl;
    }
}

int main(){
    vector<int> processes = {1, 2, 3};
    int n = processes.size();

    vector<int> burst_time = {10, 5, 8};
    int quantum = 2;
    findAvgTime(processes, n, burst_time, quantum);

    return 0;
}