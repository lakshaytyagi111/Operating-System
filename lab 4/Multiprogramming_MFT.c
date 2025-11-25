#include <stdio.h>

int main() {
    int total_mem, block_size, num_blocks, num_procs;
    int proc_mem[20];
    int allocated[20];
    int i, j;

    printf("Enter total memory (in bytes): ");
    scanf("%d", &total_mem);

    printf("Enter block size (in bytes): ");
    scanf("%d", &block_size);

    num_blocks = total_mem / block_size;
    int block_free[50];        
    for (i = 0; i < num_blocks; i++)
        block_free[i] = 1;

    printf("Enter number of processes: ");
    scanf("%d", &num_procs);

    for (i = 0; i < num_procs; i++) {
        printf("Enter memory required for process %d (in bytes): ", i + 1);
        scanf("%d", &proc_mem[i]);
        allocated[i] = -1;
    }

    int total_internal_frag = 0;

    for (i = 0; i < num_procs; i++) {
        if (proc_mem[i] > block_size) {
            allocated[i] = -1;
        } else {
            for (j = 0; j < num_blocks; j++) {
                if (block_free[j] == 1) {
                    block_free[j] = 0;
                    allocated[i] = j;           
                    total_internal_frag += block_size - proc_mem[i];
                    break;
                }
            }
        }
    }

    printf("\nTotal memory: %d bytes", total_mem);
    printf("\nBlock size : %d bytes", block_size);
    printf("\nNumber of blocks: %d\n", num_blocks);

    printf("\nPNo\tMemReq\tBlock\tAllocated?\tIntFrag");
    printf("\n-----------------------------------------------------");
    for (i = 0; i < num_procs; i++) {
        printf("\n%d\t%d\t", i + 1, proc_mem[i]);
        if (allocated[i] == -1) {
            printf("-\tNO\t\t-");
        } else {
            int frag = block_size - proc_mem[i];
            printf("%d\tYES\t\t%d", allocated[i], frag);
        }
    }

    printf("\n\nTotal internal fragmentation = %d bytes\n", total_internal_frag);

    int free_blocks = 0;
    for (i = 0; i < num_blocks; i++)
        if (block_free[i] == 1) free_blocks++;
    int external_waste = free_blocks * block_size;
    printf("Unused free memory in whole blocks (external waste) = %d bytes\n",
           external_waste);

    return 0;
}
