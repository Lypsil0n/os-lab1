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


    for (int current_row = 0; current_row < 100000; current_row++) {
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
            int index = 0;
            int furthest = 0;
            if(empty_pages){
                index = no_physical_pages - empty_pages;
                empty_pages--;
            } else {
                for(int j = 0; j < no_physical_pages; j++){
                    found = 0;
                    for(int k = current_row + 1; k < 100000; k++){
                        if (accesses[k] / page_size == buffer[j]) {
                            found = 1;
                            if(k > furthest){
                                furthest = k;
                                index = j;
                            }
                            break;
                        }
                    }
                    if(!found){
                        index = j;
                        break;
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
