#include <stdio.h>

int main() {
    int n, frames, i, j, k, page_faults = 0, flag;
    printf("Enter number of pages in reference string: ");
    scanf("%d", &n);
    int refString[n];
    printf("Enter page reference string:\n");
    for(i = 0; i < n; i++)
        scanf("%d", &refString[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    int frame[frames];

    for(i = 0; i < frames; i++)
        frame[i] = -1;

    int front = 0;
    printf("\nPage Replacement Process:\n");
    for(i = 0; i < n; i++) {
        flag = 0;
        for(j = 0; j < frames; j++) {
            if(frame[j] == refString[i]) {
                flag = 1; 
                break;
            }
        }
        if(flag == 0) { 
            frame[front] = refString[i];
            front = (front + 1) % frames;
            page_faults++;

            printf("Page %d caused a page fault. Frames: ", refString[i]);
            for(k = 0; k < frames; k++) 
                if(frame[k] != -1) printf("%d ", frame[k]);
            printf("\n");
        }
    }
    printf("Total Page Faults = %d\n", page_faults);
    printf("Page Fault Rate = %.2f\n", (float)page_faults / n);

    return 0;
}
