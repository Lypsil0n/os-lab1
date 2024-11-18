#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct trace{
    int reference;
    int next_hit;
};

int main(int argc, char** argv) {
    int no_physical_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];
    struct trace *buffer = (struct trace*)malloc(no_physical_pages * sizeof(struct trace));
    int *accesses = (int*)malloc(no_physical_pages * sizeof(int));
    int page_faults = 0;
    int current_row;
    unsigned int address;

    FILE *file = fopen(filename, "r");

    int i = 0;
    while (fscanf(file, "%d", &accesses[i]) != EOF){
        i++;
    }
    fclose(file);

    for (int i = 0; i < no_physical_pages; i++) {
        buffer[i].reference = -1;
        buffer[i].next_hit = -1;
    }

    for (current_row = 0; current_row < 100000; current_row++) {
        int page = accesses[current_row] / page_size;
        int found = 0;

        for(int i = 0; i < no_physical_pages; i++) {
            if(buffer[i].reference == page) {
                found = 1;
                break;
            }
        }

        if(!found){
            page_faults++;
            int empty_slot_index;
            int empty_slots = 0;

            for(empty_slot_index = 0; empty_slot_index < no_physical_pages; empty_slot_index++) {
                if(buffer[empty_slot_index].reference == -1) {
                    empty_slots = 1;
                    break;
                }
            }

            int next_hit = INT_MAX;
            for(int i = current_row + 1; i < 100000; i++){
                if(accesses[i] / page_size == page){
                    next_hit = i;
                    break;
                }
            }

            if(empty_slots){
                buffer[empty_slot_index].reference = page;
                buffer[empty_slot_index].next_hit = next_hit;
            } else {
                int furthest_index = 0;
                int furthest_entry = buffer[0].next_hit;

                for(int i = 1; i < no_physical_pages; i++){
                    if(buffer[i].next_hit > furthest_entry){
                        furthest_index = i;
                        furthest_entry = buffer[i].next_hit;
                    }
                }
                
                buffer[furthest_index].reference = page;
                buffer[furthest_index].next_hit = next_hit;
            }
        }
    }

    free(buffer);

    printf("No physical pages = %d, page size = %d\n", no_physical_pages, page_size);
    printf("Number of page faults: %d\n", page_faults);

	return 0;
}
