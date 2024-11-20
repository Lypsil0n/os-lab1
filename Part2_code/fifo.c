#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int no_physical_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];
    int *fifo_queue = (int*)malloc(no_physical_pages * sizeof(int));
    int page_faults = 0;
    int size = 0;
    int first = 0;
    int address;

    FILE *file = fopen(filename, "r");

    while (fscanf(file, "%d", &address) != EOF) {
        int page = address / page_size;
        int found = 0;

        for (int i = 0; i < size; i++) {
            if (fifo_queue[(first + i) % no_physical_pages] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            page_faults++;

            if (size < no_physical_pages) {
                fifo_queue[(first + size) % no_physical_pages] = page;
                size++;
            } else {
                fifo_queue[first] = page;
                first = (first + 1) % no_physical_pages;
            }
        }
    }

    free(fifo_queue);
    fclose(file);

    printf("No physical pages = %d, page size = %d\n", no_physical_pages, page_size);
    printf("Number of page faults: %d\n", page_faults);

    return 0;
}
