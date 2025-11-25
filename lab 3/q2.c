#include <stdio.h>
#include <stdlib.h>



int main(){
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    
    
    int m;
    printf("Enter the number of resources: ");
    scanf("%d" , &m);

    int available[m];
    for (int i = 0; i < m; i++){
        printf("enter the number of resources of type %d:", i);
        scanf("%d", &available[i]);
    }
    int max[n][m];
    for(int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            printf("enter the number of resources of type %d required by process %d: ", j, i);
            scanf("%d", &max[i][j]);
        }
    }

    int allocation[n][m];
    for (int i = 0; i  <n; i++){
        for (int j = 0; j < m; j++){
            printf("enter the number of resources of type %d allocated to process %d: ", j, i);
            scanf("%d", &max[i][j]);
        }
    }

    int need[n][m];
	int finish[n];
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			need[i][j] = max[i][j] - allocation[i][j];
		}
		finish[i] = 0;
	}

    int work[m];
	for (int j = 0; j < m; j++){
		work[j] = available[j];
	}

    int changed = 1;

    while (changed){
        changed = 0;

        for (int i = 0; i < n; i ++){
            if (finish[i] == 0){
                int canRun = 1;
            

                for (int j = 0; j < m; j++){
                    if (need[i][j] > work[j]){
                        canRun = 0;
                        break;
                    }
                }
                if (canRun){
                    for (int j = 0; j < n; j++){
                        available[j] += allocation[i][j];
                    }
                    finish[i] = 1;
                    changed = 1;
                }
            }
        }
    }

    int deadlock = 0;
    for (int i = 0; i < n; i++){
        if (finish[i] == 0){
            deadlock = 1;
            break;
        }
    }
    if (deadlock){
        printf("There is a deadlock bitch!");
    }

    return 0;
}