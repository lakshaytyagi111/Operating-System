#include <stdio.h>

typedef struct {
    int base;
    int limit;
} Segment;

int main() {
    int n, i;
    printf("Enter number of segments: ");
    scanf("%d", &n);
    Segment segmentTable[n];

    printf("Enter base and limit for each segment:\n");
    for(i = 0; i < n; i++) {
        printf("Segment %d base: ", i);
        scanf("%d", &segmentTable[i].base);
        printf("Segment %d limit: ", i);
        scanf("%d", &segmentTable[i].limit);
    }

    int segNum, offset;
    char choice = 'y';
    while(choice == 'y' || choice == 'Y') {
        printf("\nEnter logical address (segment number and offset): ");
        scanf("%d%d", &segNum, &offset);

        if(segNum >= n) {
            printf("Invalid segment number!\n");
        } else if(offset >= segmentTable[segNum].limit) {
            printf("Offset exceeds segment limit!\n");
        } else {
            int physicalAddr = segmentTable[segNum].base + offset;
            printf("Physical address = %d\n", physicalAddr);
        }

        printf("Translate another address (y/n)? ");
        scanf(" %c", &choice);
    }

    return 0;
}
