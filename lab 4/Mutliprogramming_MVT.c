#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int pid;
	int start;
	int size;
	int req_size;
} block;


block *blocks = NULL;
int block_count = 0;
int total_memory = 0;
int quantum = 4;
int next_pid = 1;

int round_up(int req, int q){
	return ((req + q - 1) / q)*q;
}

void coalese_holes(){
	for (int i = 0; i < block_count ; i++){
		if (blocks[i].pid == -1 && blocks[i+1].pid == -1){
			blocks[i].size += blocks[i+1].size;
		for (int j = i+2; j < block_count - 1; j++){
			blocks[j] = blocks[j+1];
		}
		block_count--;
		i--;
		}		
	}
}

void display_memory(){
	int total_frags = 0;
	int used = 0;
	
	for(int i = 0; i < block_count; i++){
		block b = blocks[i];
		int end = b.start + b.size - 1;
		printf("[%4d - %4d] size=%4d ", b.start, end, b.size);
		
		if (b.size == -1){
			printf("Hole");
		} else {
			int frag = b.size - b.req_size;
			printf("PID = %d Req = %d Frag = %d\n", b.pid , b.req_size, frag);
			used += b.size;
			total_frags += frag;
		}

		printf("-----------------------------------------------\n");
		printf("Total Memory = %d\n", total_memory);
		printf("Used Memory  = %d\n", used);
		printf("Free Memory  = %d\n", total_memory - used);
		printf("Total Internal Fragmentation = %d\n", total_frags);
		printf("-----------------------------------------------\n");

	}
}

int allocate_process(int req){
	int alloc_size = round_up(req, quantum);

	for (int i= 0; i < block_count; i++){
		if (blocks[i].pid == -1 && blocks[i].size >= alloc_size){
			
			if (blocks[i].size == alloc_size) {
				blocks[i].pid = next_pid;
				blocks[i].req_size = req;
				return next_pid++;
			}
			
			int original_size = blocks[i].size;
			int start_addr = blocks[i].start;

			blocks[i].pid = next_pid;
			blocks[i].start = start_addr;
			blocks[i].size = alloc_size;
			blocks[i].req_size = req;

			block new_hole;
			new_hole.pid = -1;
			new_hole.start = start_addr + alloc_size;
			new_hole.size = original_size - alloc_size;	
			new_hole.req_size = 0;		
		
			
			blocks = realloc(blocks, sizeof(block)*(block_count + 1));
			for (int j = block_count; j > i + 1; j--){
				blocks[j] = blocks[j - 1];
				blocks[i+1] = new_hole;
				block_count++;

				return next_pid++;
			}
		}
	}
	return -1;
}

int deallocate_process(int pid){
	for (int i = 0; i < block_count; i++){
		if (blocks[i].pid == pid){
			blocks[i].pid = -1;
			blocks[i].req_size = 0;
			coalese_holes();
			return 0;
		}
	}
	return -1;
}

int main(){
	int choice, req, pid;

	printf("Enter total memory size: ");
	scanf("%d", &total_memory);

	printf("Enter allocation quantum (e.g., 1, 4, 8): ");
	scanf("%d", &quantum);
	blocks = malloc(sizeof(block));
    blocks[0].pid = -1;
    blocks[0].start = 0;
    blocks[0].size = total_memory;
    blocks[0].req_size = 0;
    block_count = 1;

    while (1) {
        printf("\nMENU:\n");
        printf("1. Allocate Process\n");
        printf("2. Deallocate Process\n");
        printf("3. Display Memory\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter required memory: ");
            scanf("%d", &req);
            int p = allocate_process(req);
            if (p == -1)
                printf("Allocation failed! Not enough contiguous memory.\n");
            else
                printf("Process allocated with PID = %d\n", p);

            display_memory();
        }

        else if (choice == 2) {
            printf("Enter PID to deallocate: ");
            scanf("%d", &pid);
            if (deallocate_process(pid) == -1)
                printf("Invalid PID or process not found!\n");
            else
                printf("Process %d deallocated.\n", pid);

            display_memory();
        }

        else if (choice == 3) {
            display_memory();
        }

        else if (choice == 4) {
            printf("Exiting.\n");
            break;
        }

        else {
            printf("Invalid choice!\n");
        }
    }

    free(blocks);
    return 0;

}
