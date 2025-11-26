#include <stdio.h>

int main() {
    int n, frames, i, j, k, page_faults = 0;
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

    printf("\nPage Replacement Process:\n");
    for(i = 0; i < n; i++) {
        int page = refString[i];
        int hit = 0;

        for(j = 0; j < frames; j++) {
            if(frame[j] == page) {
                hit = 1;
                break;
            }
        }

        if(hit == 0) {
            page_faults++;
            int replaceIndex = -1;
            int farthest = i;

            for(j = 0; j < frames; j++) {
                if(frame[j] == -1) {
                    replaceIndex = j;
                    break;
                }
                int nextUse = n;
                for(k = i + 1; k < n; k++) {
                    if(refString[k] == frame[j]) {
                        nextUse = k;
                        break;
                    }
                }
                if(nextUse > farthest) {
                    farthest = nextUse;
                    replaceIndex = j;
                }
            }
            frame[replaceIndex] = page;

            printf("Page %d caused a page fault. Frames: ", page);
            for(j = 0; j < frames; j++)
                if(frame[j] != -1) printf("%d ", frame[j]);
            printf("\n");
        }
    }

    printf("Total Page Faults = %d\n", page_faults);
    printf("Page Fault Rate = %.2f\n", (float)page_faults / n);

    return 0;
}
