#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int main(int argc, char** argv) {
    int no_physical_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];
    int *buffer = (int*)malloc(no_physical_pages * sizeof(int));
    int *accesses = (int*)malloc(100000 * sizeof(int));
    int page_faults = 0;
    int current_row;
    unsigned int address;
    int found;
    int page;
    int empty_pages = no_physical_pages;

    FILE *file = fopen(filename, "r");

    int i = 0;
    while (fscanf(file, "%d", &accesses[i]) != EOF){
        i++;
    }
    fclose(file);

    for (int i = 0; i < no_physical_pages; i++) {
        buffer[i] = -1;
    }

    for (current_row = 0; current_row < 100000; current_row++) {
        page = accesses[current_row] / page_size;
        found = 0;

        for(int i = 0; i < no_physical_pages; i++){
            if(buffer[i] == page){
                found = 1;
                break;
            }
        }

        if(!found){
            page_faults++;
            int index;
            if(empty_pages){
                index = no_physical_pages - empty_pages;
                empty_pages--;
            } else {
                int index = -1;
                int furthest = -1;
                for(int j = 0; j < no_physical_pages; j++){
                    found = 0;
                    for(int k = current_row + 1; k < 100000; k++){
                            found = 1;
                            if (furthest < k)
                            {
                                furthest = k;
                                index = j;
                            }
                            break;
                    }
                    if(!found){
                        furthest = 100000;
                        index = j;
                    }
                }
            }
            buffer[index] = page;
        }

    }

    free(buffer);
    free(accesses);

    printf("No physical pages = %d, page size = %d\n", no_physical_pages, page_size);
    printf("Number of page faults: %d\n", page_faults);

	return 0;
}
