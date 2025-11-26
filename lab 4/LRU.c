#include <stdio.h>

int main() {
    int n, frames, i, j, k, l, page_faults = 0;
    printf("Enter number of pages: ");
    scanf("%d", &n);
    int refString[n];
    printf("Enter page reference string:\n");
    for (i = 0; i < n; i++)
        scanf("%d", &refString[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);
    int frame[frames], time[frames];
    for (i = 0; i < frames; i++) {
        frame[i] = -1;
        time[i] = 0;
    }

    int count = 0;
    printf("\nPage Replacement Process:\n");
    for (i = 0; i < n; i++) {
        int page = refString[i];
        int hit = 0;

        for (j = 0; j < frames; j++) {
            if (frame[j] == page) {
                hit = 1;
                count++;
                time[j] = count; // update recent use time
                break;
            }
        }

        if (hit == 0) {
            page_faults++;
            int lru_index = 0;
            int min_time = time[0];

            for (k = 1; k < frames; k++) {
                if (time[k] < min_time) {
                    min_time = time[k];
                    lru_index = k;
                }
            }

            frame[lru_index] = page;
            count++;
            time[lru_index] = count;

            printf("Page %d caused a page fault. Frames: ", page);
            for (l = 0; l < frames; l++)
                if (frame[l] != -1) printf("%d ", frame[l]);
            printf("\n");
        }
    }
    printf("Total Page Faults = %d\n", page_faults);
    printf("Page Fault Rate = %.2f\n", (float)page_faults / n);

    return 0;
}
