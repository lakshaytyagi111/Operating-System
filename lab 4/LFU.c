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

    int frame[frames], freq[frames], time[frames];
    for(i = 0; i < frames; i++) {
        frame[i] = -1;
        freq[i] = 0;
        time[i] = 0;
    }

    int currentTime = 0;
    printf("\nPage Replacement Process:\n");
    for(i = 0; i < n; i++) {
        int page = refString[i];
        int hitIndex = -1;
        currentTime++;

        for(j = 0; j < frames; j++) {
            if(frame[j] == page) {
                hitIndex = j;
                break;
            }
        }

        if(hitIndex != -1) {
            freq[hitIndex]++;
            time[hitIndex] = currentTime;
        } else {
            page_faults++;
            int lfu_index = -1, min_freq = 1e9, min_time = 1e9;

            for(j = 0; j < frames; j++) {
                if(frame[j] == -1) { 
                    lfu_index = j;
                    break;
                }
                if(freq[j] < min_freq) {
                    min_freq = freq[j];
                    min_time = time[j];
                    lfu_index = j;
                } else if(freq[j] == min_freq && time[j] < min_time) {
                    min_time = time[j];
                    lfu_index = j;
                }
            }
            frame[lfu_index] = page;
            freq[lfu_index] = 1;
            time[lfu_index] = currentTime;

            printf("Page %d caused a page fault. Frames: ", page);
            for(k = 0; k < frames; k++)
                if(frame[k] != -1) printf("%d ", frame[k]);
            printf("\n");
        }
    }
    printf("Total Page Faults = %d\n", page_faults);
    printf("Page Fault Rate = %.2f\n", (float)page_faults / n);
    return 0;
}
