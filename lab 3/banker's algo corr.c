#include <stdio.h>
#include <stdlib.h>

int main() {

    int n; // number of processes
    printf("Enter the number of processes in total: ");
    scanf("%d", &n);

    int m; // number of resource types
    printf("Enter the number of resource types in total: ");
    scanf("%d", &m);

    int available[m];
    for (int i = 0; i < m; i++) {
        printf("Enter the number of resources of type %d: ", i);
        scanf("%d", &available[i]);
    }

    int max[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("Enter the max number of resources of type %d required by process %d: ", j, i);
            scanf("%d", &max[i][j]);
        }
    }

    int allocation[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("Enter the number of resources of type %d allocated to process %d: ", j, i);
            scanf("%d", &allocation[i][j]);
        }
    }

    // Compute need matrix
    int need[n][m];
    int finish[n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
        finish[i] = 0; // not finished yet
    }

    // Work = available
    int work[m];
    for (int j = 0; j < m; j++) {
        work[j] = available[j];
    }

    int safeSeq[n];
    int count = 0; // how many processes finished

    while (count < n) {
        int found = 0; // to track if we found at least one process that can finish

        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) { // not finished
                int canAllocate = 1;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = 0; // not enough resources
                        break;
                    }
                }

                if (canAllocate) {
                    // Process can safely finish
                    for (int j = 0; j < m; j++)
                        work[j] += allocation[i][j];

                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (found == 0) {
            printf("\nSystem is NOT in a safe state!\n");
            return 0;
        }
    }

    // If reached here, safe sequence found
    printf("\nSystem is in a SAFE state.\nSafe Sequence: ");
    for (int i = 0; i < n; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return 0;
}
