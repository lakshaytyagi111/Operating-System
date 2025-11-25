#include <stdio.h>

int main() {
    int nb, np;
    int i, j;

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);

    int blockSize[50], blockFree[50];
    for (i = 0; i < nb; i++) {
        printf("Enter size of block %d: ", i + 1);
        scanf("%d", &blockSize[i]);
        blockFree[i] = 1; 
    }

    printf("Enter number of processes: ");
    scanf("%d", &np);

    int processSize[50], allocation[50], fragmentation[50];
    for (i = 0; i < np; i++) {
        printf("Enter size of process %d: ", i + 1);
        scanf("%d", &processSize[i]);
        allocation[i] = -1;    
        fragmentation[i] = 0;
    }

    for (i = 0; i < np; i++) {
        int worstIndex = -1;

        for (j = 0; j < nb; j++) {
            if (blockFree[j] && blockSize[j] >= processSize[i]) {
                if (worstIndex == -1 || blockSize[j] > blockSize[worstIndex]) {
                    worstIndex = j;
                }
            }
        }

        if (worstIndex != -1) {
            allocation[i] = worstIndex;                      
            fragmentation[i] = blockSize[worstIndex] - processSize[i];
            blockSize[worstIndex] -= processSize[i];          
            
        }
    }

    printf("\nProcess\tPSize\tBlock\tFrag");
    printf("\n----------------------------------");
    for (i = 0; i < np; i++) {
        printf("\n%d\t%d\t", i + 1, processSize[i]);
        if (allocation[i] == -1) {
            printf("NotAlloc\t-");
        } else {
            printf("%d\t%d", allocation[i] + 1, fragmentation[i]);
        }
    }

    int total_free = 0;
    for (i = 0; i < nb; i++) {
        total_free += blockSize[i];
    }
    printf("\n\nTotal free / fragmented memory = %d\n", total_free);

    return 0;
}

