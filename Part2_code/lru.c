#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trace{
    int reference;
    int last_used;
};

int main(int argc, char** argv) {
    int no_physical_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];
    struct trace *lru_queue = (struct trace*)malloc(no_physical_pages * sizeof(struct trace));
    int page_faults = 0;
    unsigned int address;

    FILE *file = fopen(filename, "r");

    for (int i = 0; i < no_physical_pages; i++) {
        lru_queue[i].reference = -1;
        lru_queue[i].last_used = -1;
    }

    while (fscanf(file, "%d", &address) != EOF) {
        int page = address / page_size;
        int found = 0;

        for(int i = 0; i < no_physical_pages; i++) {
            if(lru_queue[i].reference == page) {
                found = 1;
                lru_queue[i].last_used = 0;
                break;
            }
        }

        if(!found){
            page_faults++;
            int empty_slot_index;
            int empty_slots = 0;

            for(empty_slot_index = 0; empty_slot_index < no_physical_pages; empty_slot_index++) {
                if(lru_queue[empty_slot_index].reference == -1) {
                    empty_slots = 1;
                    break;
                }
            }

            if(empty_slots){
                lru_queue[empty_slot_index].reference = page;
                lru_queue[empty_slot_index].last_used = 0;
            } else {
                int oldest_index = 0;
                int oldest_age = lru_queue[0].last_used;

                for(int i = 1; i < no_physical_pages; i++){
                    if(lru_queue[i].last_used > oldest_age){
                        oldest_index = i;
                        oldest_age = lru_queue[i].last_used;
                    }
                }
                
                lru_queue[oldest_index].reference = page;
                lru_queue[oldest_index].last_used = 0;
            }
        }

        for(int i = 0; i < no_physical_pages; i++) {
            if(lru_queue[i].reference != -1 && lru_queue[i].reference != page) {
                lru_queue[i].last_used++;
            }
        }
    }

    free(lru_queue);
    fclose(file);

    printf("No physical pages = %d, page size = %d\n", no_physical_pages, page_size);
    printf("Number of page faults: %d\n", page_faults);

	return 0;
}
