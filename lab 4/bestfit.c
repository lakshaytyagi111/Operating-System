#include <stdio.h>

#define MAX 50

int main() {
    int nb, np;
    int i, j;

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);

    int blockSize[MAX], blockOriginal[MAX], blockAllocated[MAX];
    for (i = 0; i < nb; i++) {
        printf("Enter size of block %d: ", i + 1);
        scanf("%d", &blockSize[i]);
        blockOriginal[i] = blockSize[i];  
        blockAllocated[i] = 0;         
    }

    printf("Enter number of processes: ");
    scanf("%d", &np);

    int processSize[MAX], allocation[MAX], fragmentation[MAX];
    for (i = 0; i < np; i++) {
        printf("Enter size of process %d: ", i + 1);
        scanf("%d", &processSize[i]);
        allocation[i] = -1;
        fragmentation[i] = 0;
    }

    for (i = 0; i < np; i++) {
        int bestIndex = -1;

        for (j = 0; j < nb; j++) {
            if (!blockAllocated[j] && blockSize[j] >= processSize[i]) {
                if (bestIndex == -1 || blockSize[j] < blockSize[bestIndex]) {
                    bestIndex = j;
                }
            }
        }

        if (bestIndex != -1) {
            allocation[i] = bestIndex;
            fragmentation[i] = blockSize[bestIndex] - processSize[i];
            blockAllocated[bestIndex] = 1; 
        }
    }

    printf("\nProcess\tPSize\tBlock\tBSize\tFrag");
    printf("\n---------------------------------------------");
    for (i = 0; i < np; i++) {
        printf("\n%d\t%d\t", i + 1, processSize[i]);
        if (allocation[i] == -1) {
            printf("NotAlloc\t-\t-");
        } else {
            int b = allocation[i];
            printf("%d\t%d\t%d", b + 1, blockOriginal[b], fragmentation[i]);
        }
    }

    int total_internal = 0;
    for (i = 0; i < np; i++) {
        total_internal += fragmentation[i];
    }
    printf("\n\nTotal internal fragmentation = %d\n", total_internal);

    int total_free = 0;
    for (i = 0; i < nb; i++) {
        if (!blockAllocated[i]) {
            total_free += blockSize[i];
        }
    }
    printf("Total external fragmentation (free but unused memory) = %d\n",
           total_free);

    return 0;
}
