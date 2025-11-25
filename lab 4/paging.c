#include <stdio.h>

#define MAX_PAGES 50

int main() {
    int logical_size, page_size;
    int num_pages, num_frames;
    int page_table[MAX_PAGES];
    int i;

    // Input sizes
    printf("Enter logical address space size (in bytes): ");
    scanf("%d", &logical_size);

    printf("Enter page/frame size (in bytes): ");
    scanf("%d", &page_size);

    // Number of logical pages
    num_pages = logical_size / page_size;
    printf("Number of logical pages = %d\n", num_pages);

    // Assume physical memory has at least as many frames as pages (for simple lab)
    printf("Enter number of frames in physical memory: ");
    scanf("%d", &num_frames);

    // Create page table: for each page, store the frame number
    printf("\nEnter frame number for each page (-1 if page not in memory):\n");
    for (i = 0; i < num_pages; i++) {
        printf("Page %d -> frame: ", i);
        scanf("%d", &page_table[i]);
    }

    // Translate logical addresses repeatedly
    while (1) {
        int logical_addr;
        int page_no, offset;
        int frame_no, physical_addr;
        char choice;

        printf("\nEnter logical address (-1 to exit): ");
        scanf("%d", &logical_addr);
        if (logical_addr < 0)
            break;

        // Check address range
        if (logical_addr >= logical_size) {
            printf("Logical address out of range!\n");
            continue;
        }

        // Find page number and offset
        page_no = logical_addr / page_size;
        offset  = logical_addr % page_size;

        frame_no = page_table[page_no];
        if (frame_no == -1) {
            printf("Page fault: page %d is not in memory.\n", page_no);
        } else {
            physical_addr = frame_no * page_size + offset;
            printf("Logical address %d -> page %d, offset %d\n",
                   logical_addr, page_no, offset);
            printf("Mapped to frame %d -> physical address %d\n",
                   frame_no, physical_addr);
        }

        printf("Do you want to translate another address? (y/n): ");
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y')
            break;
    }

    return 0;
}
